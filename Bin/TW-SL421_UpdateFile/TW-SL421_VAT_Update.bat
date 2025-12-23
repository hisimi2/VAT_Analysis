@Echo Off
copy sqlite3.exe c:\windows\system32

sqlite3 ..\\sys_data\\TW_LH_VAT.db3 < DB_VAT.sql
