NAME = blitter

GAME_C_FILES = main.c intro.c ninja_bg.c sounds.c lib/blitter/blitter.c lib/blitter/blitter_tmap.c lib/blitter/blitter_sprites.c lib/sampler/sampler.c 
GAME_BINARY_FILES = ninja_bg.tmap ninja_bg.tset ninja.spr sawDisc_1.spr


include $(BITBOX)/kernel/bitbox.mk
main.c: ninja_bg.tmap ninja.spr sawDisc_1.spr

%.tset %.tmap %.c %.h: %.tmx
	python tmx.py $< > $*.h

#piece.spr : piece/p_?.png
	#python $(BITBOX)/lib/b#litter/scripts/couples_encode.py $@ $^

ninja.spr : ninja/ninja_??.png
	python $(BITBOX)/lib/blitter/scripts/couples_encode.py $@ $(sort $^)
	
sawDisc_1.spr : hurtobjects/sawDisc_??.png
	python $(BITBOX)/lib/blitter/scripts/couples_encode.py $@ $(sort $^)

#cursor.spr : cursor.png
	#python $(BITBOX)/lib/blitter/scripts/couples_encode.py $@ $^

sounds.c: sounds/*.wav mk_sounds.py
	python mk_sounds.py $< > $@

#bitbox_icon.c: icon.png
#	python $(BITBOX)/2nd_boot/mk_ico.py $^ > $@

clean::
	rm -f *.tset *.tmap *.spr
	rm -f jumper3.[ch] intro_tmap.[ch] cote.[ch] bitbox_icon.c
