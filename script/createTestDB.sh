#!/bin/bash


function fatal_error() {
if [ $# -gt 0 ]; then
echo $1
fi
echo "Aborting..."
exit 1
}
function print_status() {
if [ $# -gt 0 ]; then
echo $1
if [ $# -gt 1 ]; then
if [ $2 = "VISUAL" ]; then
type convert >/dev/null 2>&1 && zenity --info --title="$0" --text="$1"
fi
fi
fi
}

PDF_DIR="`readlink -f "../database/pdf"`"
DB_DIR="`readlink -f "../database"`"
TEST_IMG_DIR="`readlink -f "../test-img-marinai"`"
SQLITE_DB="../test/database_test.db"


# INITIALIZE SQULITE3 DB
print_status "Reinitializing SQLITE TEST-DB." DEBUG
if [ -f "$SQLITE_DB" ];then
rm "$SQLITE_DB"
fi
cat /dev/null > "$SQLITE_DB"
echo "CREATE TABLE test_images (id_test INTEGER PRIMARY KEY,name varchar,path varchar,name_of_pdf varchar,number_of_pages int,x1 int,y1 int,x2 int,y2 int,z int);">/tmp/sqlitetmpfile
sqlite3 "$SQLITE_DB" < /tmp/sqlitetmpfile

# INSERT DATA OF THE TEST_IMAGES
sqlite3 "$SQLITE_DB" "INSERT INTO test_images (name,path,name_of_pdf,number_of_pages,x1,y1,x2,y2,z) VALUES ('test-4520a037_page1.jpg','$TEST_IMG_DIR/','$PDF_DIR/4520a037.pdf','1',0,0,0,0,0)";

sqlite3 "$SQLITE_DB" "INSERT INTO test_images (name,path,name_of_pdf,number_of_pages,x1,y1,x2,y2,z) VALUES ('test-4520a099_page4.jpg','$TEST_IMG_DIR/','$PDF_DIR/4520a099.pdf','4',0,0,0,0,0)";

sqlite3 "$SQLITE_DB" "INSERT INTO test_images (name,path,name_of_pdf,number_of_pages,x1,y1,x2,y2,z) VALUES ('test-4520a207_page0.jpg','$TEST_IMG_DIR/','$PDF_DIR/4520a207.pdf','0',0,0,0,0,0)";

sqlite3 "$SQLITE_DB" "INSERT INTO test_images (name,path,name_of_pdf,number_of_pages,x1,y1,x2,y2,z) VALUES ('test-4520a409_page1.jpg','$TEST_IMG_DIR/','$PDF_DIR/4520a409.pdf','1',0,0,0,0,0)";

sqlite3 "$SQLITE_DB" "INSERT INTO test_images (name,path,name_of_pdf,number_of_pages,x1,y1,x2,y2,z) VALUES ('test-4520a678_formula_page3.jpg','$TEST_IMG_DIR/','$PDF_DIR/4520a678.pdf','3',0,0,0,0,0)";

sqlite3 "$SQLITE_DB" "INSERT INTO test_images (name,path,name_of_pdf,number_of_pages,x1,y1,x2,y2,z) VALUES ('test-4520a678_immagine_page1.jpg','$TEST_IMG_DIR/','$PDF_DIR/4520a678.pdf','1',0,0,0,0,0)";

sqlite3 "$SQLITE_DB" "INSERT INTO test_images (name,path,name_of_pdf,number_of_pages,x1,y1,x2,y2,z) VALUES ('test-4520a678_paragrafo2_page3.jpg','$TEST_IMG_DIR/','$PDF_DIR/4520a678.pdf','3',0,0,0,0,0)";

sqlite3 "$SQLITE_DB" "INSERT INTO test_images (name,path,name_of_pdf,number_of_pages,x1,y1,x2,y2,z) VALUES ('test-4520a678_paragrafo3_page3.jpg','$TEST_IMG_DIR/','$PDF_DIR/4520a678.pdf','3',0,0,0,0,0)";

sqlite3 "$SQLITE_DB" "INSERT INTO test_images (name,path,name_of_pdf,number_of_pages,x1,y1,x2,y2,z) VALUES ('test-4520a678_paragrafo_page1.jpg','$TEST_IMG_DIR/','$PDF_DIR/4520a678.pdf','1',0,0,0,0,0)";

sqlite3 "$SQLITE_DB" "INSERT INTO test_images (name,path,name_of_pdf,number_of_pages,x1,y1,x2,y2,z) VALUES ('test-4520a957_formula_page1.jpg','$TEST_IMG_DIR/','$PDF_DIR/4520a957.pdf','1',0,0,0,0,0)";

sqlite3 "$SQLITE_DB" "INSERT INTO test_images (name,path,name_of_pdf,number_of_pages,x1,y1,x2,y2,z) VALUES ('test-4520a957_frase_page1.jpg','$TEST_IMG_DIR/','$PDF_DIR/4520a957.pdf','1',0,0,0,0,0)";

sqlite3 "$SQLITE_DB" "INSERT INTO test_images (name,path,name_of_pdf,number_of_pages,x1,y1,x2,y2,z) VALUES ('test-4520b235_page3.jpg','$TEST_IMG_DIR/','$PDF_DIR/4520b235.pdf','3',0,0,0,0,0)";

sqlite3 "$SQLITE_DB" "INSERT INTO test_images (name,path,name_of_pdf,number_of_pages,x1,y1,x2,y2,z) VALUES ('test-4520b235_paragrafo2_page3.jpg','$TEST_IMG_DIR/','$PDF_DIR/4520b235.pdf','3',0,0,0,0,0)";

sqlite3 "$SQLITE_DB" "INSERT INTO test_images (name,path,name_of_pdf,number_of_pages,x1,y1,x2,y2,z) VALUES ('test-4520z029_page0.jpg','$TEST_IMG_DIR/','$PDF_DIR/4520z029.pdf','0',0,0,0,0,0)";

