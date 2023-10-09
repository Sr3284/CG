#!/bin/bash
# install.sh

declare -a libs=("mesa-utils"
				 "libglu1-mesa-dev"
				 "freeglut3-dev"
				 "mesa-common-dev"
				 "libglew-dev"
				 "libxmu-dev"
				 "libxi-dev")

# Atualiza a lista de pacotes
sudo apt-get update

echo

# Verifica os pacotes e instala caso já não esteja
for i in "${libs[@]}"
do
	if [ $(dpkg-query -W -f='${Status}\n' $i 2>/dev/null | grep -c "install ok installed") -eq 0 ]
	then
		echo "$i não encontrada. Instalando $i."
		sudo apt-get install $i
	else
		echo "$i já instalada"
	fi
done