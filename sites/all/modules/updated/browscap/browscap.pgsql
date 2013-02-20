CREATE TABLE browscap (
  useragent varchar(255) PRIMARY KEY,
  parent varchar(64) NOT NULL,
  browser varchar(64) NOT NULL,
  version varchar(8) NOT NULL,
  majorversion varchar(8) NOT NULL,
  minorversion varchar(8) NOT NULL,
  platform varchar(8) NOT NULL,
  authenticodeupdate varchar(8) NOT NULL,
  cssversion tinyint(4) DEFAULT 0 NOT NULL,
  frames tinyint(4) DEFAULT 0 NOT NULL,
  iframes tinyint(4) DEFAULT 0 NOT NULL,
  htmltables tinyint(4) DEFAULT 0 NOT NULL,
  cookies tinyint(4) DEFAULT 0 NOT NULL,
  backgroundsounds tinyint(4) DEFAULT 0 NOT NULL,
  vbscript tinyint(4) DEFAULT 0 NOT NULL,
  javascript tinyint(4) DEFAULT 0 NOT NULL,
  javaapplets tinyint(4) DEFAULT 0 NOT NULL,
  activexcontrols tinyint(4) DEFAULT 0 NOT NULL,
  cdf tinyint(4) DEFAULT 0 NOT NULL,
  aol tinyint(4) DEFAULT 0 NOT NULL,
  beta tinyint(4) DEFAULT 0 NOT NULL,
  win16 tinyint(4) DEFAULT 0 NOT NULL,
  crawler tinyint(4) DEFAULT 0 NOT NULL,
  stripper tinyint(4) DEFAULT 0 NOT NULL,
  wap tinyint(4) DEFAULT 0 NOT NULL,
  netclr tinyint(4) DEFAULT 0 NOT NULL
);

CREATE TABLE browscap_statistics (
  parent VARCHAR(255) PRIMARY KEY,
  counter integer DEFAULT 0 NOT NULL,
  is_crawler smallint DEFAULT 0 NOT NULL
);
