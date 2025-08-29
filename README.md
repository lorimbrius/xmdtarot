# XmDtarot
X11/Motif graphical version of Discordian Tarot

## Usage
xmdtarot

Draws your (or someone else's) super-helpful Discordian Tarot reading.

The spread is drawn as a pentagon with the positions of True on top, Seek next, then False, then Avoid, and, finally, Meaningless. See if you can spot the pattern!

## Hotkeys
<kbd>Ctrl</kbd> + <kbd>M</kbd> - show putative card meanings below cards. Of course, you're free to add whatever meaning you'd like; all things are true|false|meaningless anyway.

<kbd>Ctrl</kbd> + <kbd>N</kbd> - draw a new spread (each second seeds the RNG so doing it too often leads to it not actually drawing a new spread)

<kbd>Ctrl</kbd> + <kbd>Q</kbd> - quit the program

## Building
Just type `make`. You'll need the Motif libraries and headers.

### Secret second deck
If you also have ImageMagick and GhostScript installed, you can build with a higher-resolution and more modern deck. Just type `make DECKV=2`. It takes longer to build because it needs to decompose the PostScript file into individual cards.

## Credits
Original Discordian Tarot Deck by Max Flax Beeblewax and (boing!) Cnoocy Mosque O'Witz (marc@suberic.net).
(NOTE: I have no idea if that e-mail address still works.)

Python program by Lord Imbrius the Despondent (darthferrett@gmail.com).
(NOTE: I also have no idea if that e-mail address still works - e-mail me and find out!)

## Note
For further enlightenment, consult your pineal gland.