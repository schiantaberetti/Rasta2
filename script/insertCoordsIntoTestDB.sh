#!/bin/bash
function calculate() {
	if [ $# -gt 0 ];then 
		echo "scale=4; $1" | bc ;
	fi
}

function display_image() {
	show_image="pippo.jpg"
	border_size=6
	borderx=` calculate "($3 + $border_size)" `
	bordery=` calculate "($4 + $border_size)" `
	`mogrify -fill red -stroke black -draw "rectangle $3,$4 $borderx,$bordery" -write $show_image $1/$2`
	borderx=` calculate "($5 + $border_size)" `
	bordery=` calculate "($6 + $border_size)" `
	`mogrify -fill red -stroke black -draw "rectangle $5,$6 $borderx,$bordery" $show_image`
	borderx=` calculate "($7 + $border_size)" `
	bordery=` calculate "($8 + $border_size)" `
	`mogrify -fill red -stroke black -draw "rectangle $7,$8 $borderx,$bordery" $show_image`
	borderx=` calculate "($9 + $border_size)" `
	bordery=` calculate "(${10} + $border_size)" `
	`mogrify -fill red -stroke black -draw "rectangle $9,${10} $borderx,$bordery" $show_image`
	borderx=` calculate "(${11} + $border_size)" `
	bordery=` calculate "(${12} + $border_size)" `
	`mogrify -fill red -stroke black -draw "rectangle ${11},${12} $borderx,$bordery" $show_image`
	display $show_image
	rm $show_image 
}

function update_row() {
	sqlite3 "$SQLITE_DB" "UPDATE test_images SET sx_x='$2',sx_y='$3',dx_x='$4',dx_y='$5',up_x='$6',up_y='$7',down_x='$8',down_y='$9',inner_x='${10}',inner_y='${11}' where name='$1'"
}

function set_parameters() {
	sx=$1
	dx=$2
	bx=` calculate "($dx + $sx)" `
	bx=`expr $bx / 2`
	by=$3
	ux=$bx
	uy=$4
	##mettere a intero
	inx=` calculate "($dx + $sx)" `
	inx=`expr $inx / 2`
	iny=` calculate "($uy + $by)" `
	iny=`expr $iny / 2`
	sy=` calculate "($by + $uy)" `
	sy=`expr $sy / 2`
	dy=$sy
	test_img=$5
	visual_img=$6
}


SQLITE_DB="../test/database_test.db"
visual_image="4520a037-1.jpg"
test_img="test-4520a037_page1.jpg"
IMG_SHOW_DIR="img_to_show"
DEBUG=0

##############  sx   dx    by    uy       test_img               visual_img
set_parameters "0" "320" "310" "265" "test-4520a037_page1.jpg" "4520a037-1.jpg"
update_row $test_img $sx $sy $dx $dy $ux $uy $bx $by $inx $iny
if [ "$DEBUG" -eq "1" ]; then
	display_image $IMG_SHOW_DIR $visual_img $sx $sy $dx $dy $ux $uy $bx $by $inx $iny
fi

set_parameters "0" "325" "475" "315" "test-4520a099_page4.jpg" "4520a099-4.jpg"
update_row $test_img $sx $sy $dx $dy $ux $uy $bx $by $inx $iny
if [ "$DEBUG" -eq "1" ]; then
	display_image $IMG_SHOW_DIR $visual_img $sx $sy $dx $dy $ux $uy $bx $by $inx $iny
fi

set_parameters "0" "605" "165" "25" "test-4520a207_page0.jpg" "4520a207-0.jpg"
update_row $test_img $sx $sy $dx $dy $ux $uy $bx $by $inx $iny
if [ "$DEBUG" -eq "1" ]; then
	display_image $IMG_SHOW_DIR $visual_img $sx $sy $dx $dy $ux $uy $bx $by $inx $iny
fi

set_parameters "315" "555" "505" "395" "test-4520a409_page1.jpg" "4520a409-1.jpg"
update_row $test_img $sx $sy $dx $dy $ux $uy $bx $by $inx $iny
if [ "$DEBUG" -eq "1" ]; then
	display_image $IMG_SHOW_DIR $visual_img $sx $sy $dx $dy $ux $uy $bx $by $inx $iny
fi

set_parameters "275" "565" "185" "95" "test-4520a678_formula_page3.jpg" "4520a678-3.jpg"
update_row $test_img $sx $sy $dx $dy $ux $uy $bx $by $inx $iny
if [ "$DEBUG" -eq "1" ]; then
	display_image $IMG_SHOW_DIR $visual_img $sx $sy $dx $dy $ux $uy $bx $by $inx $iny
fi

set_parameters "275"  "595" "545" "455" "test-4520a678_paragrafo3_page3.jpg" "4520a678-3.jpg"
update_row $test_img $sx $sy $dx $dy $ux $uy $bx $by $inx $iny
if [ "$DEBUG" -eq "1" ]; then
	display_image $IMG_SHOW_DIR $visual_img $sx $sy $dx $dy $ux $uy $bx $by $inx $iny
fi

set_parameters "0" "325" "595" "475" "test-4520a678_paragrafo2_page3.jpg" "4520a678-3.jpg"
update_row $test_img $sx $sy $dx $dy $ux $uy $bx $by $inx $iny
if [ "$DEBUG" -eq "1" ]; then
	display_image $IMG_SHOW_DIR $visual_img $sx $sy $dx $dy $ux $uy $bx $by $inx $iny
fi

set_parameters "165" "315" "155" "65" "test-4520a678_immagine_page1.jpg" "4520a678-1.jpg"
update_row $test_img $sx $sy $dx $dy $ux $uy $bx $by $inx $iny
if [ "$DEBUG" -eq "1" ]; then
	display_image $IMG_SHOW_DIR $visual_img $sx $sy $dx $dy $ux $uy $bx $by $inx $iny
fi

set_parameters "275" "575" "705" "625" "test-4520a678_paragrafo_page1.jpg" "4520a678-1.jpg"
update_row $test_img $sx $sy $dx $dy $ux $uy $bx $by $inx $iny
if [ "$DEBUG" -eq "1" ]; then
	display_image $IMG_SHOW_DIR $visual_img $sx $sy $dx $dy $ux $uy $bx $by $inx $iny
fi

set_parameters "195" "335" "155" "85" "test-4520a957_formula_page1.jpg" "4520a957-1.jpg"
update_row $test_img $sx $sy $dx $dy $ux $uy $bx $by $inx $iny
if [ "$DEBUG" -eq "1" ]; then
	display_image $IMG_SHOW_DIR $visual_img $sx $sy $dx $dy $ux $uy $bx $by $inx $iny
fi

set_parameters "35" "205" "705" "645" "test-4520a957_frase_page1.jpg" "4520a957-1.jpg"
update_row $test_img $sx $sy $dx $dy $ux $uy $bx $by $inx $iny
if [ "$DEBUG" -eq "1" ]; then
	display_image $IMG_SHOW_DIR $visual_img $sx $sy $dx $dy $ux $uy $bx $by $inx $iny
fi

set_parameters "45" "305" "225" "155" "test-4520b235_page3.jpg" "4520b235-3.jpg"
update_row $test_img $sx $sy $dx $dy $ux $uy $bx $by $inx $iny
if [ "$DEBUG" -eq "1" ]; then
	display_image $IMG_SHOW_DIR $visual_img $sx $sy $dx $dy $ux $uy $bx $by $inx $iny
fi

set_parameters "285" "595" "535" "385" "test-4520b235_paragrafo2_page3.jpg" "4520b235-3.jpg"
update_row $test_img $sx $sy $dx $dy $ux $uy $bx $by $inx $iny
if [ "$DEBUG" -eq "1" ]; then
	display_image $IMG_SHOW_DIR $visual_img $sx $sy $dx $dy $ux $uy $bx $by $inx $iny
fi

set_parameters "155" "445" "165" "75" "test-4520z029_page0.jpg" "4520z029.jpg"
update_row $test_img $sx $sy $dx $dy $ux $uy $bx $by $inx $iny
if [ "$DEBUG" -eq "1" ]; then
	display_image $IMG_SHOW_DIR $visual_img $sx $sy $dx $dy $ux $uy $bx $by $inx $iny
fi
