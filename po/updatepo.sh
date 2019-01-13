#!bash

GITVER=`git describe --always --dirty`
GITHASH=`git rev-parse HEAD`
PROJ=hardinfo
SRC_DIRS="hardinfo/ shell/ modules/ includes/ deps/verbose-spork/sysobj/src/ deps/verbose-spork/gui/"

echo "Update gettext translation files."

DER=`pwd`
if [ ! -e "updatepo.sh" ]
then
    echo "Error: Run from po/, the location of $PROJ.pot and XX.po files."
    exit 1
fi

MSGTOTALOLD=`msgattrib --untranslated $PROJ.pot | grep -E "^msgstr \"\"" | wc -l`
echo "$PROJ.pot has $MSGTOTALOLD strings"

mv $PROJ.pot $PROJ.pot.old
echo "" > $PROJ.pot # empty existing file to join (-j) with
for d in $SRC_DIRS;
do
    # work from src root to get reasonable file reference comments
    cd ..
    echo -n `pwd`; echo "/$d ..."
    find "$d" -type f -name "*.[hc]" -print | sort | xargs xgettext -j -d $PROJ -o "$DER/$PROJ.pot" -k_ -kN_ -kC_:1c,2 -kNC_:1c,2 -c/ --from-code=UTF-8
    cd "$DER"
done;

MSGTOTAL=`msgattrib --untranslated $PROJ.pot | grep -E "^msgstr \"\"" | wc -l`
CMSG=`msgattrib --untranslated $PROJ.pot | grep -E "^#,.*c-format" | wc -l`

TDIFF=$(($MSGTOTAL - $MSGTOTALOLD))
CHANGE="$TDIFF"
if [ $TDIFF -gt 0 ]; then CHANGE="+$TDIFF"; fi
if [ $TDIFF -eq 0 ]; then CHANGE="no change"; fi
echo "$PROJ.pot now has $MSGTOTAL strings ($CHANGE), with $CMSG c-format strings"
echo "(as of $GITVER $GITHASH)"

for f in *.po
do
    cp "$f" "$f.old"

    msgmerge -q -N "$f" $PROJ.pot > tmp.po

    # set/reset the X-Poedit-Basepath header
    grep -v '"X-Poedit-Basepath\:[^"]*"' tmp.po | sed 's:\("Language\:[^"]*"\):\1\n"X-Poedit-Basepath\: ../\\n":' >"$f"

    rm -f tmp.po

    # stats
    UNMSG=`msgattrib --untranslated "$f" | grep -E "^msgstr \"\"" | wc -l`
    FUZMSG=`msgattrib --translated "$f" | grep -E "^#,.*fuzzy" | wc -l`
    DONE=" "; if [ $UNMSG -eq 0 ]; then DONE="x"; fi
    echo "- [$DONE] $f : ($UNMSG / $MSGTOTAL remain untranslated, needs work/fuzzy: $FUZMSG)"
done
