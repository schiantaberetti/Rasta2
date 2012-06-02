#!/bin/bash
DB_DIR="`readlink -f "../database"`"
EXT="jpg"
SQLITE_DB="database.db"
SIFT_CMD="../tools/img2sifts"
JOBS=0
CONCURRENCY=1


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
function sqlite3_query(){
	sql_db=$1
	query=$2
	while ! sqlite3 "$1" "$2" >/dev/null 2>/dev/null ; do
    	sleep 1
	done
}
function extract_sift_job() {
	while [ $JOBS -ge $CONCURRENCY ]; do sleep 1; done;
	let JOBS=JOBS+1
	
	image_l=$1 # INPUT
	id_pages_l=$2 # INPUT
	
	sift_file_name="`echo $image | cut -d '.' -f 1`.sift"
	sift_file_name="`readlink -f $DB_DIR/sift/`/$sift_file_name"

	$SIFT_CMD `readlink -f "$DB_DIR/pdf_img/$image_l"` $sift_file_name
	sqlite3_query "$DB_DIR/$SQLITE_DB" "INSERT INTO sifts (name,path,id_pages) VALUES ('`echo $image_l | cut -d '.' -f 1`.sift','$DB_DIR/sift/','$id_pages_l')";
	let JOBS=JOBS-1
}

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
	mkdir  "$DB_DIR/sift"
fi
if [ ! -d "$DB_DIR/pdf_img" ];then
	mkdir  "$DB_DIR/pdf_img"
fi

# Converting pdf to images
id_paper=1;
id_pages=1;
for pdf_file in `ls -1 $DB_DIR/pdf/*.pdf | sed 's#.*/##' `; do
	print_status "Processing: $pdf_file" DEBUG;
	sqlite3_query "$DB_DIR/$SQLITE_DB" "INSERT INTO papers (id_paper,name,path) VALUES ('$id_paper','$pdf_file','$DB_DIR/pdf/')";
	
	convert "$DB_DIR/pdf/$pdf_file" -alpha off "$DB_DIR/pdf_img/`echo $pdf_file | cut -d '.' -f 1`.$EXT"
	
	pdf_basename=`echo $pdf_file| cut -d '.' -f 1`
	# Extracting SIFT
	for image in `ls -1  $DB_DIR/pdf_img/$pdf_basename*.$EXT | sed 's#.*/##' `; do
		print_status "Extracting SIFT from: $image" DEBUG;
		num_page=`echo $image | sed 's/[^0-9]*//' | cut -d '.' -f 1`
		
		sqlite3_query "$DB_DIR/$SQLITE_DB" "INSERT INTO pages (number_of_page,id_paper,path, name, id_pages) VALUES ('$num_page','$id_paper','$DB_DIR/pdf_img/','$image','$id_pages')";

		extract_sift_job $image $id_pages & 		
		
		let id_pages=id_pages+1
	done
	let id_paper=id_paper+1
done


#print_status "DB creation complete. :-)" VISUAL

