if [ -f "../build/confi" ]; then
	../build/confi test.confi
else
	echo "Binary file (confi) not found"
fi
