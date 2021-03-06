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

DB_DIR="`readlink -f "../database"`"
EXT="jpg"
SQLITE_DB="database.db"
SIFT_CMD="../tools/img2sifts"
if [ $# -gt 0 ]; then
DB_DIR=`readlink $1`
fi
print_status "Analyzing database in directory: $DB_DIR" DEBUG

# Error handling
if [ ! -d "$DB_DIR/pdf" ];then
fatal_error "Folder <<$DB_DIR/pdf>> doesn't exist."
fi
if [ ! `ls -A $DB_DIR/pdf/*.pdf` ];then
fatal_error "The directory $DB_DIR/pdf is empty, there is no documents to process."
fi
if [ ! -x $SIFT_CMD ];then
fatal_error "Can't find the image to sift text converter file: $SIFT_CMD."
fi

type convert >/dev/null 2>&1 || fatal_error "Can't find the convert program. You should install ImageMagick."
type sqlite3 >/dev/null 2>&1 || fatal_error "Can't find the sqlite3 program. You should install sqlite3."

# DELETE OLD FILES
if [ -d $DB_DIR/pdf_img ];then
rm -R $DB_DIR/pdf_img
fi
if [ -d $DB_DIR/sift ];then
rm -R $DB_DIR/sift
fi

# INITIALIZE SQULITE3 DB
print_status "Reinitializing SQLITE DB." DEBUG
if [ -f "$DB_DIR/$SQLITE_DB" ];then
rm "$DB_DIR/$SQLITE_DB"
fi
cat /dev/null > "$DB_DIR/$SQLITE_DB"
echo "CREATE TABLE papers (id_paper INTEGER PRIMARY KEY,name varchar,path varchar);">/tmp/sqlitetmpfile
echo "CREATE TABLE pages (number_of_page integer,id_paper integer, path varchar, name varchar, id_pages integer primary key,FOREIGN KEY(id_paper) REFERENCES papers(id_paper));">>/tmp/sqlitetmpfile
echo "CREATE TABLE sifts (id_sift integer primary key,name varchar,path varchar, id_pages integer,FOREIGN KEY(id_pages) REFERENCES pages(id_pages));">>/tmp/sqlitetmpfile
sqlite3 "$DB_DIR/$SQLITE_DB" < /tmp/sqlitetmpfile


# Creating necessary folders
if [ ! -d "$DB_DIR/sift" ];then
mkdir "$DB_DIR/sift"
fi
if [ ! -d "$DB_DIR/pdf_img" ];then
mkdir "$DB_DIR/pdf_img"
fi

# Converting pdf to images
id_paper=1;
id_pages=1;
for pdf_file in `ls -1 $DB_DIR/pdf/*.pdf | sed 's#.*/##' `; do
print_status "Processing: $pdf_file" DEBUG;
sqlite3 "$DB_DIR/$SQLITE_DB" "INSERT INTO papers (id_paper,name,path) VALUES ('$id_paper','$pdf_file','$DB_DIR/pdf/')";

convert "$DB_DIR/pdf/$pdf_file" -alpha off "$DB_DIR/pdf_img/`echo $pdf_file | cut -d '.' -f 1`.$EXT"

pdf_basename=`echo $pdf_file| cut -d '.' -f 1`
# Extracting SIFT
for image in `ls -1 $DB_DIR/pdf_img/$pdf_basename*.$EXT | sed 's#.*/##' `; do
print_status "Extracting SIFT from: $image" DEBUG;

#extract page number
OLD_IFS="$IFS"
IFS="-"
STR_ARRAY=( $image )
IFS="$OLD_IFS"

OLD_IFS="$IFS"
IFS="."
num_page=( ${STR_ARRAY[1]} )
IFS="$OLD_IFS"

#num_page=`echo $image | sed 's/[^0-9]*//' | cut -d '.' -f 1`

sqlite3 "$DB_DIR/$SQLITE_DB" "INSERT INTO pages (number_of_page,id_paper,path, name, id_pages) VALUES ('$num_page','$id_paper','$DB_DIR/pdf_img/','$image','$id_pages')";

sift_file_name="`echo $image | cut -d '.' -f 1`.sift"
sift_file_name="`readlink -f $DB_DIR/sift/`/$sift_file_name"

$SIFT_CMD `readlink -f "$DB_DIR/pdf_img/$image"` $sift_file_name
sqlite3 "$DB_DIR/$SQLITE_DB" "INSERT INTO sifts (name,path,id_pages) VALUES ('`echo $image | cut -d '.' -f 1`.sift','$DB_DIR/sift/','$id_pages')";

let id_pages=id_pages+1
done
let id_paper=id_paper+1
done


#print_status "DB creation complete. :-)" VISUAL
