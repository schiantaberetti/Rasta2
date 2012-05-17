#!/bin/bash
i=1
for file in `ls paper-*`;do
	#echo $file
	part=`echo $file | cut -d '-' -f 1`
	ext=`echo $file | cut -d '.' -f 2`
	mv $file "$part-page_$i.$ext"
	let i=i+1	
done
