#!/bin/sh
old_version='v0.7'
new_version='v0.8.1'
Date=`date +"%b %d, %Y"`
Year=`date +"%Y"`
header="
 * \\date        $Date
 * \\version     ${new_version}
 * \\copyright   <2009-$Year> Forschungszentrum Jülich GmbH. All rights reserved.
 *
 * \\section License
 * This file is part of JuPedSim.
 *
 * JuPedSim is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * JuPedSim is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with JuPedSim. If not, see <http://www.gnu.org/licenses/>.
 **/
"

for f in $(find . -name '*.cpp' -or -name '*.h' );  
do 

#file uptodate
if grep -R -q "$new_version" $f; then
	echo "File $f is uptodate"
	continue
fi

if grep -R -q "\version     $old_version" $f; then
	echo "Processing $f  $old_version ---> $new_version"
	sed -i -e "s/${old_version}/${new_version}/g" $f
else
    echo "File $f does not contain a version number. Adding header.."
    VAR="/**
 * \\file        `basename $f` $header
     "
    ed -s "$f" << EOF
0a
$VAR
.
w
EOF
fi
done

