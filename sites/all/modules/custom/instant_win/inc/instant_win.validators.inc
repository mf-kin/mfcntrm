<?php

/**
 *
 * Validates code based on the following rules:
 *   returns true if the code has not already been used or if was used by the same user in a different day, otherwise
 *   returns false.
 * 
 * @param type $code
 * @param type $user_email
 * @return type 
 */
function instant_win_validate_code($code, $user_email = NULL) {
  $query = db_select('instant_win_contest_codes', 'cc')
        ->fields('cc', array('code', 'is_alternate'));
  $query->leftJoin('instant_win_contest_entries', 'ce', 'ce.contest_code = cc.code');
  $query->fields('ce', array('email', 'registered_at'));
  $query->leftJoin('instant_win_alternate_codes', 'ac', 'ac.contest_code = cc.code');
  $query->fields('ac', array('email_sent_at'));
  $query->condition('cc.code', $code);
  $query->orderBy('registered_at', 'DESC');
  $query->forUpdate();
  $query->range(0, 1);
  
  $queryResult = $query->execute()->fetch();
  
  $result = INSTANT_WIN_INVALID_CODE;
  
  $code_exists = $queryResult !== FALSE;
  
  $unsed_code = $code_exists && empty($queryResult->email);
  
  $same_user =  !$unsed_code && $queryResult->email == $user_email;
  
  $not_same_day = $code_exists && $queryResult->registered_at < mktime(0, 0, 0);
  
  $alternate_sent = $code_exists && 
    (!$queryResult->is_alternate || $queryResult->is_alternate && $queryResult->email_sent_at > 0);

  if ($code_exists && $same_user && !$not_same_day) {
    $result = INSTANT_WIN_USED_ON_SAME_DAY;
  }
  else if (($unsed_code || $same_user) && $alternate_sent) {
    $result = INSTANT_WIN_VALID_CODE;
  }
  
  return $result;
}



function instant_win_is_total_codes_taken_by_an_user_exeeded($element, &$form_state) {
  $email = $element['#value'];
  $maximum_allowed_codes = variable_get('instant_win_register_limit_user_get_a_code', 10);
  
  module_load_include('inc', 'instant_win', 'inc/instant_win.common');
  instant_win_create_transaction_on_form_state($form_state);
  $total_codes_taken_by_an_user = instant_win_get_total_codes_taken_by_an_user($email);
  
  return $total_codes_taken_by_an_user >= $maximum_allowed_codes;
}

function instant_win_get_total_codes_taken_by_an_user($email) {
  $query1 = _instant_win_get_query_for_all_unique_code_entries_from_an_user($email);
  $query2 = _instant_win_get_query_for_all_unused_alternate_codes_requested_by_user($email);
  
  $query2->addExpression('1');
  $query1->union($query2, 'UNION ALL');
  
  $total_codes = $query1->countQuery()->execute()->fetchField();
  
  return $total_codes;
}

function _instant_win_get_query_for_all_unused_alternate_codes_requested_by_user($email) {
  $query = db_select('instant_win_alternate_codes', 'iwac')
        ->fields('iwac', array('contest_code'));
  $query->leftJoin('instant_win_contest_entries', 'iwce', 'iwce.contest_code = iwac.contest_code');
  $query->condition('iwac.email', $email, '=');
  $query->condition('iwce.email', NULL, 'IS');
  
  $query->distinct();
  
  return $query;
}

function _instant_win_get_query_for_all_unique_code_entries_from_an_user($email) {
  $query = db_select('instant_win_contest_entries', 'iwce')
        ->fields('iwce', array('contest_code'));
  $query->condition('iwce.email', $email, '=');
  $query->orderBy('registered_at', 'DESC');
  
  $query->distinct();
  
  return $query;
}

/**
 * Validates a name using the regex set to instant_win_first_name_validation_pattern Drupal's variable.
 * 
 * @param string $name
 * @return boolean 
 */
function instant_win_validate_first_name($name) {
  $validation_pattern = variable_get('instant_win_first_name_validation_pattern', '');
  
  return _instant_win_pregmatch_if_pattern_not_empty($validation_pattern, $name);
}

/**
 * Validates a name using the regex set to instant_win_last_name_validation_pattern Drupal's variable.
 * 
 * @param string $name
 * @return boolean 
 */
function instant_win_validate_last_name($name) {
  $validation_pattern = variable_get('instant_win_last_name_validation_pattern', '');

  return _instant_win_pregmatch_if_pattern_not_empty($validation_pattern, $name);
}

/**
 * Checks whether the given regex matches the given value
 * If the regex is empty, then no validation is performed and returns TRUE.
 * 
 * Warning: This function is not intended to be called directly by functions other than
 * instant_win_validate_first_name() and instant_win_validate_last_name()
 * 
 * @param string $name
 * @return boolean 
 */
function _instant_win_pregmatch_if_pattern_not_empty($validation_pattern, $value) {
  $is_valid = TRUE;
  if (!empty($validation_pattern)) {
    $is_valid = preg_match($validation_pattern, $value);
  }
  
  return $is_valid;
}

function instant_win_validate_zip_code($zip_code) {
  $validation_pattern = variable_get('instant_win_zipcode_validation_pattern', '');

  return _instant_win_pregmatch_if_pattern_not_empty($validation_pattern, $zip_code);
}

function instant_win_validate_phone_number($phone_number) {
  $validation_pattern = variable_get('instant_win_phone_validation_pattern', '');

  return _instant_win_pregmatch_if_pattern_not_empty($validation_pattern, $phone_number);
}

function instant_win_validate_dob($dob) {
  $datetime_dob = new DateTime("{$dob['year']}-{$dob['month']}-01");
  $datetime_now = new DateTime('now');
  $years_difference = $datetime_dob->diff($datetime_now)->format('%R%y');

  // The returned value $interval->format('%R%y') is a string with the range (+) ou (-) and the difference in years
  // Check if the (+) sign is on the string, just to avoid some injection  
  $is_valid_date_interval = preg_match('/^\+[0-9]+$/', $years_difference);

  $is_valid_date_of_birth = FALSE;
  if ($is_valid_date_interval) {
    $minimum_allowed_age = variable_get('instant_win_dob_min_age_to_register', 18);
    
    $is_valid_date_of_birth = (int)$years_difference >= $minimum_allowed_age;
  }

  return $is_valid_date_of_birth;
}


function instant_win_validate_code_limit_by_user_per_day($email) {
  $today = mktime(0, 0, 0);
  
  $query = db_select('instant_win_contest_entries', 'ce')
         ->condition('email', $email, '=')
         ->condition('registered_at', $today, '>');
  
  $query->addExpression('count(email)', 'email_count');
  $query->forUpdate();
  
  $register_count = $query->execute()
                  ->fetchField();
  $limit = variable_get('instant_win_register_limit_per_day','');
  
  return (int)$register_count < $limit;  
}


function instant_win_verify_household($user) {
  $today = mktime(0,0,0);

  $query = db_select('instant_win_winners', 'w');
  $query->addExpression('count(w.email)');
  $query->condition('household', $user->house_hold)
        ->condition('user_registered_at', $today, '>=');
  
  $result = $query->execute()->fetchField();
  
  return (boolean)!($result);
}
