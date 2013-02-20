<?php

/**
 * Gets the user object based on the registration code.
 *
 * @param type $code
 * @return stdClass
 */
function instant_win_get_user_by_code($code) {
  $user = FALSE;

  $query = db_select('instant_win_contest_entries', 'ce')
          ->fields('ce', array('ceid', 'email', 'contest_code', 'registered_at', 'processed_at'));
  $query->innerJoin('instant_win_contest_codes', 'cc', 'cc.code = ce.contest_code');
  $query->condition('ce.contest_code', $code);

  $query_result = $query->execute()->fetchAllAssoc('ceid', PDO::FETCH_ASSOC);

  if (!empty($query_result)) {
    $user = new stdClass();
    foreach ($query_result as $ceid => $row) {
      $user->email = $row['email'];
      $user->contest_code = $row['contest_code'];
      $user->entries[$ceid] = array(
        'registered_at' => $row['registered_at'],
        'processed_at'  => $row['processed_at'],
      );
    }
  }

  return $user;
}

/**
 * Return configured main registration form success page.
 */
function instant_win_register_form_success_page() {
  $default_value = array(
    'value'  => '',
    'format' => ''
  );
  $body = variable_get('instant_win_new_registration_form_success_page_content', $default_value);
  $body['value'] = check_markup($body['value'], $body['format']);

  return $body['value'];
}

/**
 *
 * return wether the for the given $email is already registered
 *
 * @param type $email the email to looking for
 * @return boolean
 */
function instant_win_is_user_registered($email) {
  $query = db_select('instant_win_users', 'i')
         ->condition('email', $email, '=');
  $query->forUpdate();

  $query->addExpression('count(1)', 'emailCount');

  $is_registered = $query->execute()
                 ->fetchField();

  return $is_registered > 0;
}

/**
 * Gets the user object by email.
 *
 * @param type $code
 * @return stdClass
 */
function instant_win_get_user_by_email($email) {
  $user = FALSE;

  $query_result = db_select('instant_win_users', 'i')
                ->fields('i')
                ->condition('email', $email, '=')
                ->execute()
                ->fetchAll();

  if (!empty($query_result)) {
    $user = new stdClass();
    foreach ($query_result as $row) {
      $user->iwuid = $row->iwuid;
      $user->first_name = $row->first_name;
      $user->last_name = $row->last_name;
      $user->address_line_1 = $row->address_line_1;
      $user->address_line_2 = $row->address_line_2;
      $user->city = $row->city;
      $user->state =$row->state;
      $user->zip = $row->zip;
      $user->email = $row->email;
      $user->telephone = $row->telephone;
      $user->dob = $row->dob;
    }
  }

  return $user;
}

/**
 *
 * Gets a alternate code that has not already been sent to a user.
 *
 * @return type
 */
function instant_win_get_available_alternate_code() {
  try {
    $query = db_select('instant_win_contest_codes', 'cc');
    $query->fields('cc', array('code'));
    $query->forUpdate();
    $query->condition('is_alternate', 1);
    $query->condition('sent_at', 0);
    $query->range(0, 1);

    $result = $query->execute()->fetch();

    return $result->code;
  }
  catch (Exception $e) {
    watchdog('instant_win', t('Unable to get available alternate code: :msg', array(':msg' => $e->getMessage())), 'error');
    throw $e;
  }
}

/**
 * Insert new entry into winners table.
 *
 * @param $fields
 *  Array containing all column fields.
 */
function instant_win_insert_winner($fields) {
  try {
    $entry = db_insert('instant_win_winners')
      ->fields($fields)
      ->execute();

    return $entry;
  }
  catch (Exception $e) {
    watchdog('instant_win', t('Winners table insertion failed: :msg', array(':msg' => $e->getMessage())), 'error');
    throw $e;
  }
}

/**
 *
 * Starts a transaction and insert it on the form_state
 *
 * @param type $form_state
 */
function instant_win_create_transaction_on_form_state(&$form_state) {
  if (!isset($form_state['transaction'])) {
    // opening a transaction.
    $form_state['transaction'] = db_transaction();
  }
}

/**
 *
 * Marks a specific contest entry as processed setting processed_at with the current date.
 *
 * @param type $ceid
 * @param int $status
 * @return boolean TRUE when the update was successful, FALSE otherwise.
 */
function instant_win_update_entry_as_processed($ceid, $status, $processed_at) {
  try {
    $update = db_update('instant_win_contest_entries')->fields(array(
        'processed_at' => $processed_at,
        'status' => $status,
    ));
    $update->condition('ceid', $ceid);
    $update->execute();

  }
  catch(Exception $e) {
    watchdog('instant_win', $e->getMessage());
    throw $e;
  }
}


function instant_win_process_contest_results($max_entries_to_process) {
  $prizes = instant_win_get_all_available_prizes($max_entries_to_process);
  $users = instant_win_get_unprocessed_users($max_entries_to_process);

  $prize_index = 0;

  foreach($users as $user) {
    $prize = isset($prizes[$prize_index]) ? $prizes[$prize_index] : NULL;
    if (instant_win_process_user($user, $prize, $max_entries_to_process) == INSTANT_WIN_STATUS_WINNER) {
      $prize_index++;
    }
  }

  return $max_entries_to_process;
}

function instant_win_process_user($user, $prize, &$max_entries_to_process) {
  module_load_include('inc', 'instant_win', 'inc/instant_win.validators');

  $transaction = db_transaction();
  try {
    $max_entries_to_process--; // process the entrie

    $result = INSTANT_WIN_STATUS_NOT_PROCESSED;

    if ($prize !== NULL && $user->registered_at >= $prize->timestamp) {
      instant_win_append_user_data($user);
      if (instant_win_verify_household($user)) {
        $result = INSTANT_WIN_STATUS_WINNER;
      }
      else {
        $result = INSTANT_WIN_STATUS_LOSER_SAME_HOUSEHOLD;
      }
    }
    else {
      $result = INSTANT_WIN_STATUS_LOSER_NOT_AVAILABLE_PRIZE;
    }

    $timestamp = time();
    instant_win_update_entry_as_processed($user->ceid, $result, $timestamp);

    if ($result == INSTANT_WIN_STATUS_WINNER) {
      instant_win_insert_winner_user($user, $prize);
      instant_win_set_prize_as_used($prize, $timestamp);
      instant_win_send_winner_email($user, $prize);
    }
    else {
      instant_win_send_loser_email($user);
    }

    return $result;
  }
  catch(Exception $e) {
    $max_entries_to_process++; // problem with process
    watchdog('instant_win', t('Error on process contest results for an user.'));
    $transaction->rollback();
  }
}

function instant_win_get_all_available_prizes($max_entries_to_process) {
  $query = db_select('instant_win_prize_timestamp', 'pt')
         ->fields('pt', array('tid', 'prize_nid', 'timestamp'))
         ->condition('pt.won_at', '0', '=')
         ->orderBy('pt.timestamp', 'ASC')
         ->range(0, $max_entries_to_process);
  
  $prizes = $query->execute()->fetchAll();

  return $prizes;
}

function instant_win_get_unprocessed_users($max_entries_to_process) {
  $query = db_select('instant_win_contest_entries', 'ce')
         ->fields('ce', array('ceid', 'iwuid', 'email', 'contest_code', 'registered_at'))
         ->condition('processed_at', 0, '=')
         ->orderBy('ce.registered_at', 'ASC')
         ->orderBy('ce.ceid', 'ASC')
         ->range(0, $max_entries_to_process);
  $users = $query->execute()->fetchAll();

  return $users;
}

function instant_win_insert_winner_user($user, $prize) {
  try {
    $winner_entry = instant_win_format_table_winners_fields($user, $prize);

    db_insert('instant_win_winners')
      ->fields($winner_entry)
      ->execute();

  }
  catch(Exception $e) {
    watchdog('instant_win', $e->getMessage());
    throw $e;
  }
}


function instant_win_format_table_winners_fields($user, $prize) {
  $formatted_timestamp = date('Ymd', $user->registered_at);

  $entry = array(
    'ceid' => $user->ceid,
    'email' => $user->email,
    'code' => $user->contest_code,
    'prize_nid' => $prize->prize_nid,
    'user_registered_at' => $user->registered_at,
    'prize_available_at' => $prize->timestamp,
    'formatted_user_registered_at' => $formatted_timestamp,
    'household' => $user->house_hold,
  );

  return $entry;
}


function instant_win_send_winner_email($user, $prize) {
  try {
    global $language;
    drupal_mail('instant_win', 'instant_win_prize_mail', $user->email, $language, array('node' => $prize->prize_nid));
  }
  catch(Exception $e) {
    watchdog('instant_win', t('Unable to send winner email'));
  }
}


function instant_win_send_loser_email($user) {
  try {
    global $language;
    drupal_mail('instant_win', 'instant_win_try_again_mail', $user->email, $language);
  }
  catch(Exception $e) {
    watchdog('instant_win', t('Unable to send "try again" email'));
  }
}

function instant_win_format_household($user) {
  $house_hold = strtolower($user->address_line_1 . $user->address_line_2 . $user->city . $user->state . $user->zip);
  return str_replace(' ', '', $house_hold);
}


/**
 *
 * Fetch all codes that weren't sent.
 *
 * @return type
 */
function instant_win_get_not_sent_alternate_codes($max_entries_to_process) {

  $email_delay = variable_get('instant_win_get_a_code_email_delay', 0);

  $query = db_select('instant_win_alternate_codes', 'ac');
  $query->fields('ac');
  $query->condition('email_sent_at', 0);
  $query->condition('requested_at', time() - $email_delay * 60 * 60, '<=');
  $query->range(0, $max_entries_to_process);

  return $query->execute()->fetchAllAssoc('acid');
}

/**
 *
 * Updates alternate codes table to set the email_sent_at with the current date.
 *
 * @param type $acid
 * @return type
 */
function instant_win_mark_alternate_code_as_sent($acid) {
  $update = db_update('instant_win_alternate_codes');
  $update->fields(array('email_sent_at' => time()));
  $update->condition('acid', $acid);

  return $update->execute();
}

/**
 * Sends the code by email for the user.
 *
 * @global type $language
 * @param type $code
 */
function instant_win_send_alternate_code_by_email($code) {
  $transaction = db_transaction();
  try {

    global $language;
    drupal_mail('instant_win', 'instant_win_get_a_code_mail', $code->email, $language,
            array('code' => $code->contest_code));

    instant_win_mark_alternate_code_as_sent($code->acid);

  } catch (Exception $e) {
    watchdog('instant_win', t('Send alternate code for user has failed (id: !id) :msg',
            array(':msg' => $e->getMessage(), '!id' => $code->acid)), 'error');
    throw $e;
  }
}

/**
 * Send alternate codes to users that didn't received yet.
 */
function instant_win_send_alternate_codes($max_entries_to_process) {
  $codes = instant_win_get_not_sent_alternate_codes($max_entries_to_process);

  foreach ($codes as $acid => $code) {
    instant_win_send_alternate_code_by_email($code);
  }
}


function instant_win_append_user_data(&$user) {
  $user_data = instant_win_get_user_by_email($user->email);

  foreach($user_data as $key => $data) {
    if (!property_exists($user, $key)) {
      $user->$key = $data;
    }
  }

  // appends the house_hold string formatted
  $user->house_hold = instant_win_format_household($user);
}

function instant_win_get_prize_timestamps($node_id) {
  $timestamps = db_select('instant_win_prize_timestamp', 'iwt')
         ->fields('iwt')
         ->condition('prize_nid', $node_id)
         ->execute()
         ->fetchAll();

  return $timestamps;
}

function instant_win_insert_prize_timestamp($node) {
  $prize_nid = $node->nid;
  $timestamps = $node->timestamps;

  foreach ($timestamps as $timestamp) {
    db_insert('instant_win_prize_timestamp')
    ->fields(array(
               'timestamp' => $timestamp,
               'prize_nid' => $prize_nid,
               'won_at'    => 0,
             ))->execute();
  }
}

function instant_win_delete_all_prize_timestamp($prize_nid) {
  db_delete('instant_win_prize_timestamp')
    ->condition('prize_nid', $prize_nid)
    ->execute();
}

function instant_win_delete_prize_timestamp($timestamp_tid) {
  $result = db_delete('instant_win_prize_timestamp')
          ->condition('tid', $timestamp_tid)
          ->execute();

  return $result;
}

function instant_win_get_all_prize_timestamps() {
  $query = db_select('instant_win_prize_timestamp', 't')
         ->fields('t')
         ->fields('n', array('title'));
  $query->join('node', 'n', 't.prize_nid = n.nid');
  $query->orderBy('t.prize_nid', 'ASC');
  $query->orderBy('t.timestamp', 'ASC');

  $timestamps = $query->execute()->fetchAll();
  return $timestamps;
}

/**
 * Return all registered users from instant_win_users table grouped by date.
 *
 * @return
 *  Object containing all registered users grouped by date.
 */
function instant_win_get_all_registered_users_per_day() {
  $query = db_select('instant_win_users', 'iwu')
         ->fields('iwu', array('formatted_user_registered_at'))
         ->fields('iwu', array('user_registered_at'));
  $query->groupBy('iwu.formatted_user_registered_at', 'ASC');
  $query->addExpression('count(formatted_user_registered_at)', 'total');

  $users_per_day = $query->execute()->fetchAll();

  return $users_per_day;
}

/**
 * Return all winners from instant_win_winners table grouped by date.
 *
 * @return
 *  Object containing all winners grouped by date.
 */
function instant_win_get_all_winners_per_day() {
  $query = db_select('instant_win_winners', 'iww')
         ->fields('iww', array('formatted_user_registered_at'))
         ->fields('iww', array('user_registered_at'));
  $query->groupBy('iww.formatted_user_registered_at', 'ASC');
  $query->addExpression('count(formatted_user_registered_at)', 'total');
  
  $users_per_day = $query->execute()->fetchAll();
  
  return $users_per_day;
}

/**
 * Return the number of available codes to be used.
 *
 * @return int the number of available codes
 */
function instant_win_get_count_available_codes() {
  $query = db_select('instant_win_contest_codes', 'iwc')
         ->condition('sent_at', 0, '=');

  $query->addExpression('count(code)', 'available_codes');

  $available_codes = $query->execute()
                    ->fetchField();

  return $available_codes;
}

function instant_win_set_prize_as_used($prize, $won_at) {
  try {
    $update = db_update('instant_win_prize_timestamp')
            ->fields(array(
              'won_at' => $won_at,              
            ));
    $update->condition('tid', $prize->tid);
    $update->execute();

  }
  catch(Exception $e) {
    watchdog('instant_win', $e->getMessage());
    throw $e;
  }
}