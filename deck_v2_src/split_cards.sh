#!/bin/sh
# Split a postscript file into individual cards
# Usage: split_cards.sh input.ps
# Example: split_cards.sh deck.ps
# Requires: ImageMagick (convert), Ghostscript (gs)

DENSITY=100
CARD_WIDTH=223
CARD_HEIGHT=347
LEFT_MARGIN=26
TOP_MARGIN=26
COL3_TOP_MARGIN=176
HGAP=2
VGAP=3

if [ "$#" -ne 1 ]; then
    echo "Usage: $0 input.ps"
    exit 1
fi

input_file="$1"

# Generate PNGs from PostScript pages
convert -density $DENSITY "$input_file" "tmp.png"
rc=$?

if [ $rc -ne 0 ]; then
    echo "Error: Failed to convert PostScript to PNG."
    exit $rc
fi

# Rename generated files to a consistent pattern
mv tmp-0.png trumps0.png
mv tmp-1.png trumps1.png
mv tmp-2.png trumps2.png
mv tmp-3.png sweets.png
mv tmp-4.png booms.png
mv tmp-5.png pungents.png
mv tmp-6.png prickles.png
mv tmp-7.png oranges.png
rm tmp-8.png

# Split the generated PNGs into individual cards
for page in *.png; do
    convert "$page" -crop ${CARD_WIDTH}x${CARD_HEIGHT}+${LEFT_MARGIN}+${TOP_MARGIN} +repage +adjoin "${page}_1.png"
    convert "$page" -crop ${CARD_WIDTH}x${CARD_HEIGHT}+${LEFT_MARGIN}+$((TOP_MARGIN + CARD_HEIGHT + VGAP)) +repage +adjoin "${page}_2.png"
    convert "$page" -crop ${CARD_WIDTH}x${CARD_HEIGHT}+${LEFT_MARGIN}+$((TOP_MARGIN + 2 * (CARD_HEIGHT + VGAP))) +repage +adjoin "${page}_3.png"
    convert "$page" -crop ${CARD_WIDTH}x${CARD_HEIGHT}+$(( LEFT_MARGIN + CARD_WIDTH + HGAP))+${TOP_MARGIN} +repage +adjoin "${page}_4.png"
    convert "$page" -crop ${CARD_WIDTH}x${CARD_HEIGHT}+$(( LEFT_MARGIN + CARD_WIDTH + HGAP))+$((TOP_MARGIN + CARD_HEIGHT + VGAP)) +repage +adjoin "${page}_5.png"
    convert "$page" -crop ${CARD_WIDTH}x${CARD_HEIGHT}+$(( LEFT_MARGIN + CARD_WIDTH + HGAP))+$((TOP_MARGIN + 2 * (CARD_HEIGHT + VGAP))) +repage +adjoin "${page}_6.png"
    convert "$page" -crop ${CARD_HEIGHT}x${CARD_WIDTH}+$(( LEFT_MARGIN + 2 * (CARD_WIDTH + HGAP)))+$COL3_TOP_MARGIN +repage +adjoin -rotate 90 "${page}_7.png"
    convert "$page" -crop ${CARD_HEIGHT}x${CARD_WIDTH}+$(( LEFT_MARGIN + 2 * (CARD_WIDTH + HGAP)))+$((COL3_TOP_MARGIN + CARD_WIDTH + HGAP)) +repage +adjoin -rotate 90 "${page}_8.png"
    convert "$page" -crop ${CARD_HEIGHT}x${CARD_WIDTH}+$(( LEFT_MARGIN + 2 * (CARD_WIDTH + HGAP)))+$((COL3_TOP_MARGIN + 2 * (CARD_WIDTH + HGAP))) +repage +adjoin -rotate 90 "${page}_9.png"
    convert "$page" -crop ${CARD_HEIGHT}x${CARD_WIDTH}+$(( LEFT_MARGIN + 2 * (CARD_WIDTH + HGAP)))+$((COL3_TOP_MARGIN + 3 * (CARD_WIDTH + HGAP))) +repage +adjoin -rotate 90 "${page}_10.png"
done

# Put suits in the proper file names
for element in sweet boom pungent prickle orange; do
    mv ${element}s.png_1.png ${element}s_2.png
    mv ${element}s.png_2.png ${element}s_1.png
    mv ${element}s.png_3.png ${element}s_z.png
    mv ${element}s.png_4.png ${element}s_o.png
    mv ${element}s.png_5.png ${element}s_4.png
    mv ${element}s.png_6.png ${element}s_3.png
    mv ${element}s.png_7.png ${element}s_a.png
    mv ${element}s.png_8.png ${element}s_c.png
    mv ${element}s.png_9.png ${element}s_s.png
    mv ${element}s.png_10.png ${element}s_d.png
done

# Put trumps in the proper file names
mv trumps0.png_1.png trumps_03.png
mv trumps0.png_2.png trumps_02.png
mv trumps0.png_3.png trumps_01.png
mv trumps0.png_4.png trumps_06.png
mv trumps0.png_5.png trumps_05.png
mv trumps0.png_6.png trumps_04.png
mv trumps0.png_7.png trumps_10.png
mv trumps0.png_8.png trumps_09.png
mv trumps0.png_9.png trumps_08.png
mv trumps0.png_10.png trumps_07.png
mv trumps1.png_1.png trumps_13.png
mv trumps1.png_2.png trumps_12.png
mv trumps1.png_3.png trumps_11.png
mv trumps1.png_4.png trumps_16.png
mv trumps1.png_5.png trumps_15.png
mv trumps1.png_6.png trumps_14.png
mv trumps1.png_7.png trumps_20.png
mv trumps1.png_8.png trumps_19.png
mv trumps1.png_9.png trumps_18.png
mv trumps1.png_10.png trumps_17.png
mv trumps2.png_1.png trumps_23.png
mv trumps2.png_2.png trumps_22.png
mv trumps2.png_3.png trumps_21.png
rm trumps2.png_4.png
rm trumps2.png_5.png
rm trumps2.png_6.png
rm trumps2.png_7.png
rm trumps2.png_8.png
rm trumps2.png_9.png
rm trumps2.png_10.png

# Clean up intermediate files
rm sweets.png booms.png pungents.png prickles.png oranges.png
rm trumps0.png trumps1.png trumps2.png

# Convert PNGs to XBMs
for img in *.png; do
    convert "$img" "$(basename $img .png).xbm"
    rm "$img"
done

# Concatenate all XBM files into a single header file
cat <<EOF >../deck_v2.h
/*-
 * SPDX-License-Identifier: CC0-1.0
 */

#ifndef _DECK_V2_H
#define _DECK_V2_H

#include "xmdtarot.h"

/*
 * The Discordian Tarot Deck v2
 * By Max Flax Beeblewax and (Boing!) Cnoocy Mosque O'Witz
 * <marc@suberic.net>
 * 
 * There are 23 trumps and five suits of 10 cards each.
 * The suits are sweets, booms, pungents, prickles, and oranges.
 * Each suit has an ace, a deuce, a trey, a four, an apostle, a cow, a day,
 * an O, a season, and a zip.
 */

#define DECK_SIZE     73
#define CARD_WIDTH   $CARD_WIDTH /* pixels */
#define CARD_HEIGHT $CARD_HEIGHT /* pixels */

EOF

cat *.xbm >> ../deck_v2.h

cat <<EOF >> ../deck_v2.h

const unsigned char *FACES[] = {
  /* TRUMPS */
  trumps_01_bits, trumps_02_bits, trumps_03_bits, trumps_04_bits,
  trumps_05_bits, trumps_06_bits, trumps_07_bits, trumps_08_bits,
  trumps_09_bits, trumps_10_bits, trumps_11_bits, trumps_12_bits,
  trumps_13_bits, trumps_14_bits, trumps_15_bits, trumps_16_bits,
  trumps_17_bits, trumps_18_bits, trumps_19_bits, trumps_20_bits,
  trumps_21_bits, trumps_22_bits, trumps_23_bits,
  /* SWEETS */
  sweets_z_bits, sweets_1_bits, sweets_2_bits, sweets_3_bits, sweets_4_bits,
  sweets_o_bits, sweets_d_bits, sweets_s_bits, sweets_c_bits, sweets_a_bits,
  /* BOOMS */
  booms_z_bits, booms_1_bits, booms_2_bits, booms_3_bits, booms_4_bits,
  booms_o_bits, booms_d_bits, booms_s_bits, booms_c_bits, booms_a_bits,
  /* PUNGENTS */
  pungents_z_bits, pungents_1_bits, pungents_2_bits, pungents_3_bits, pungents_4_bits,
  pungents_o_bits, pungents_d_bits, pungents_s_bits, pungents_c_bits, pungents_a_bits,
  /* PRICKLES */
  prickles_z_bits, prickles_1_bits, prickles_2_bits, prickles_3_bits, prickles_4_bits,
  prickles_o_bits, prickles_d_bits, prickles_s_bits, prickles_c_bits, prickles_a_bits,
  /* ORANGES */
  oranges_z_bits, oranges_1_bits, oranges_2_bits, oranges_3_bits, oranges_4_bits,
  oranges_o_bits, oranges_d_bits, oranges_s_bits, oranges_c_bits, oranges_a_bits
};

const struct Card DECK[] = {
    /* TRUMPS */
    {"I The Sacred Cow", "a sacred cow"},
    {"II A Pope", "querent, or anybody"},
    {"III The Initiate", "one who does not know and seeks knowledge"},
    {"IV The Illuminate", "one who knows or pretends to and dispenses knowledge"},
    {"V The Believer", "one who accepts ideas"},
    {"VI The Skeptic", "one who rejects ideas"},
    {"VII The Flower", "sex, drugs, hedonism"},
    {"VIII The Trout", "silliness, humor, laughter"},
    {"IX Conspiracy", "there\'s a plot behind coincidences"},
    {"X Joker", "wild card, could mean anything"},
    {"XI Net of Synchronicity", "there\'s a cosmic force behind coincidences"},
    {"XII Discordian Deck", "this deck or self-referentialism"},
    {"XIII The Big Bang", "eristic creation"},
    {"XIV The Great Pyramid", "aneristic creation"},
    {"XV Radioactivity", "aneristic destruction"},
    {"XVI The Tower of Babel", "eristic destruction"},
    {"XVII The Discordian Society", "an eristic group or groups"},
    {"XVIII The Bavarian Illuminati", "an aneristic group or groups"},
    {"XIX Greyface", "someone on the all-order trip"},
    {"XX Eris", "someone on the all-disorder trip"},
    {"XXI The Golden Apple", "eristic forces"},
    {"XXII The Pentagon", "aneristic forces"},
    {"XXIII The Sacred Chao", "all things in balance"},
    /* SWEETS */
    {"Zip of Sweets", "a lack of taste, water, pleasure"},
    {"Ace of Sweets", "a good amount of taste, water, pleasure"},
    {"Deuce of Sweets", "shared or balanced taste, water, pleasure"},
    {"Trey of Sweets", "imbalance of taste, water, pleasure"},
    {"4 of Sweets", "excess taste, water, pleasure"},
    {"O of Sweets", "a moment of taste, water, pleasure"},
    {"Sweetmorn", "a physical morning or the happy beginning of something"},
    {"Chaos", "no illusions"},
    {"Cow of Sweets", "someone in the midst of, but unaffected by, taste, water, pleasure"},
    {"Hung Mung", "someone who embodies taste, water, pleasure"},
    /* BOOMS */
    {"Zip of Booms", "a lack of hearing, air, action"},
    {"Ace of Booms", "a good amount of hearing, air, action"},
    {"Deuce of Booms", "shared or balanced hearing air, action"},
    {"Trey of Booms", "imbalance of hearing, air, action"},
    {"4 of Booms", "excess hearing, air, action"},
    {"O of Booms", "a moment of hearing, air, action"},
    {"Boomtime", "a hectic morning or progress toward something"},
    {"Discord", "eristic illusions"},
    {"Cow of Booms", "someone in the midst of, but unaffected by, hearing, air, action"},
    {"Dr. Van Van Mojo", "someone who embodies hearing, air, action"},
    /* PUNGENTS */
    {"Zip of Pungents", "a lack of smell, aether, intensity"},
    {"Ace of Pungents", "a good amount of smell, aether, intensity"},
    {"Deuce of Pungents", "shared or balanced smell, aether, intensity"},
    {"Trey of Pungents", "imbalance of smell, aether, intensity"},
    {"4 of Pungents", "excess smell, aether, intensity"},
    {"O of Pungents", "a moment of smell, aether, intensity"},
    {"Pungenday", "the noon meditation or the midpoint of something"},
    {"Confusion", "conflicting illusions"},
    {"Cow of Pungents", "someone in the midst of, but unaffected by, smell, aether, intensity"},
    {"St. Gulik", "someone who embodies smell, aether, intensity"},
    /* PRICKLES */
    {"Zip of Prickles", "a lack of touch, earth, conformity"},
    {"Ace of Prickles", "a good amount of touch, earth, conformity"},
    {"Deuce of Prickles", "shared or balanced touch, earth, conformity"},
    {"Trey of Prickles", "imbalance of touch, earth, conformity"},
    {"4 of Prickles", "excess touch, earth, conformity"},
    {"O of Prickles", "a moment of touch, earth, conformity"},
    {"Prickle-Prickle", "a physical afternoon or a break from something"},
    {"Bureaucracy", "aneristic illusions"},
    {"Cow of Prickles", "someone in the midst of, but unaffected by, touch, earth, conformity"},
    {"Zarathud", "someone who embodies touch, earth, conformity"},
    /* ORANGES */ 
    {"Zip of Oranges", "a lack of sight, fire, bizarreness"},
    {"Ace of Oranges", "a good amount of sight, fire, bizarreness"},
    {"Deuce of Oranges", "shared or balanced sight, fire, bizarreness"},
    {"Trey of Oranges", "imbalance of sight, fire, bizarreness"},
    {"4 of Oranges", "excess of sight, fire, bizarreness"},
    {"O of Oranges", "a moment of sight, fire, bizarreness"},
    {"Setting Orange", "a physical evening or the celebration at the end of something"},
    {"The Aftermath", "playing with illusions"},
    {"Cow of Oranges", "someone in the midst of, but unaffected by, sight, fire, bizarreness"},
    {"The Elder Malaclypse", "someone who embodies sight, fire, bizarreness"}
};

const char DECK_ABOUT_STRING[] = "Deck and art Kopyleft (K) 2011 Max Flax Beeblewax\n"
                                 "and (Boing!) Cnoocy Mosque O'Witz (marc@suberic.net)";

#endif /* !_DECK_V2_H */
EOF

rm *.xbm