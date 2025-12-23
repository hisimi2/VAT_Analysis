BEGIN TRANSACTION;
DROP TABLE IF EXISTS "BARCODE_2D";
CREATE TABLE IF NOT EXISTS "BARCODE_2D" (
	"id"	INTEGER,
	"read_time"	DATETIME,
	"index_arm_no"	INTEGER,
	"site_no"	INTEGER,
	"barcode"	CHAR(128),
	"barcode_bak"	CHAR(128),
	"teststart_time"	DATETIME,
	"testend_time"	DATETIME,
	"result_bin"	INTEGER,
	PRIMARY KEY("id")
);
COMMIT;
