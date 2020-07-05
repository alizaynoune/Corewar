#!/bin/bash
specifier=$@
char="ali test"
flag1=("0" "+" "-" "#" "N")
flag2=("0" "+" "-" "#" "S" "N")
dot=("." "0")
length=("0" "l" "ll" "h" "hh")
gcc main.c -L ../ -lftdprintf -I ../includes -o dprintf
echo "" >error
if [ "$specifier" == "" ]
then
	specifier="c s d i o u x X p"
fi

for specif in $specifier
do
	for len in ${length[@]}
	do
		add=2
		if [ "$len" == "0" ]
		then
			len=""
		fi
		for preci in {0..4}
		do
			for dot_ in ${dot[@]}
			do
				for wid in {0..4}
				do
					nb=$(($wid * $add * $wid + ($wid * $add)))
					add=$((($add * 4) | $wid))
					if [ $wid == 4 ]
					then
						wid=""
					fi
					if [ "$preci" == "4" ]
					then
						preci=""
					fi
					for f2 in ${flag2[@]}
					do
						if [ "$f2" == "N" ]
						then
							f2=""
						fi
						if [ "$f2" == "S" ]
						then
							f2=" "
						fi
						for f1 in ${flag1[@]}
						do
							if [ "$f1" == "N" ]
							then
								f1=""
							fi
							if [ "$specif" == "s" ] || [ "$specif" == "p" ]
							then
								is="s"
								nb="test dprintf"
							else
								is="d"
							fi
							if [ "$dot_" == "0" ]
							then
								dot_=""
							fi
							./dprintf "$is" "test %${f2}${f1}${preci}${dot_}${wid}${preci}${len}${specif} end" "$nb" 1>diff
							line=$(head -1 diff)
							line2=$(tail -1 diff)
							if [ "$line" != "$line2" ]
							then
								echo -e "\e[31m \"test %${f2}${f1}${preci}${dot_}${wid}${preci}${len}${specif}\" "$nb"  [KO]\e[0m"
								echo "\"test %${f2}${f1}${preci}${dot_}${wid}${preci}${len}${specif} end\" "$nb"" >> error
								echo "   your out :[$line]" >>error
								echo "   sys out  :[$line2]" >>error
							else
								echo -e "\e[32m \"test %${f2}${f1}${preci}${dot_}${wid}${preci}${len}${specif} end\" "$nb"  [OK]\e[0m"
							fi
#							sleep 0.05
						done
					done
				done
			done
		done
		if [ "$specif" == "s" ] || [ "$specif" == "c" ]
		then
			break
		fi
	done


done
err=$(cat error | grep "sys" | wc -l)
if [ $err != 0 ]
then
	echo -e "\n\e[31m you have $err error\n\e[32mfor more ditail \"cat file error\"\e[0m\n"
fi
rm -f diff
