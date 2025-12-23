@Echo Off
copy sqlite3.exe c:\windows\system32

sqlite3 ..\\sys_data\\TW_LH.db3 < TBL_VAT_BASE_INFO.sql
