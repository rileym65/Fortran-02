PROJECT=forlib.lib
OBJS=\
	abs16.prg \
	abs32.prg \
	absfp.prg \
	add16.prg \
	add32.prg \
	add32i.prg \
	addfp.prg \
	addfpi.prg \
	addtows.prg \
	alloc.prg \
	and16.prg \
	and32.prg \
	atof.prg \
	atoi16.prg \
	atoi32.prg \
	checkeof.prg \
	checkeom.prg \
	cmp32.prg \
	comp32.prg \
	dealloc.prg \
	dec32.prg \
	div16.prg \
	div32.prg \
	div32i.prg \
	divfp.prg \
	divfpi.prg \
	epush.prg \
        eq16.prg \
        eq32.prg \
        eqfp.prg \
	fact.prg \
	false16.prg \
	false32.prg \
	fclose.prg \
	fdata.prg \
	fenter.prg \
	ferrao.prg \
	ferrno.prg \
	ffwrite.prg \
	ffread.prg \
	fildes.prg \
	fmtread.prg \
	fmtwrite.prg \
	fopen.prg \
	fpacos.prg \
	fpargs.prg \
	fpasin.prg \
	fpatan.prg \
	fpcomp2.prg \
	fpcopy.prg \
	fpdata.prg \
	fpexp.prg \
	fpln.prg \
	fpnorm.prg \
	fpos.prg \
	fpcos.prg \
	fppow.prg \
	fpret0.prg \
	fpreta.prg \
	fpretb.prg \
	fpsin.prg \
	fpsqrt.prg \
	fptan.prg \
	fread.prg \
	free.prg \
	frewind.prg \
	fromsci.prg \
	fseek.prg \
	ftoa.prg \
	ftoi.prg \
	fwrite.prg \
	getargs.prg \
        gt16.prg \
        gt32.prg \
        gte16.prg \
        gte32.prg \
        gtefp.prg \
        gtfp.prg \
	heap.prg \
	hfree.prg \
	icomp32.prg \
	ioflag.prg \
	ioresults16.prg \
	ioresults32.prg \
	iostatus.prg \
	isnumeral.prg \
	itoa16.prg \
	itoa32.prg \
	itof.prg \
	left.prg \
	lfsr.prg \
	lfsrshift.prg \
	lt16.prg \
	lt32.prg \
	lte16.prg \
	lte32.prg \
	ltefp.prg \
	ltfp.prg \
	mdnorm16.prg \
	mid.prg \
	mod16.prg \
	mod32.prg \
	mul16.prg \
	mul32.prg \
	mul32i.prg \
	mulfp.prg \
	mulfpi.prg \
        ne16.prg \
        ne32.prg \
        nefp.prg \
        neg32.prg \
        next.prg \
        next32.prg \
        nextvar.prg \
        nextvar32.prg \
	null32.prg \
	or16.prg \
	or32.prg \
	right.prg \
	rnd16.prg \
	rnd32.prg \
	round45.prg \
	scratch.prg \
	sgn16.prg \
	sgn32.prg \
	sgnfp.prg \
	shl32.prg \
	shr32.prg \
	strcat.prg \
	strcmp.prg \
	strcpy.prg \
	strcpyin.prg \
	strlen.prg \
	sub16.prg \
	sub32.prg \
	sub32i.prg \
	subfp.prg \
	subfpi.prg \
	tobcd16.prg \
	tobcd32.prg \
	tolower.prg \
	toupper.prg \
	true16.prg \
	true32.prg \
	uwrite.prg \
	uread.prg \
	vpush8p.prg \
	vpush16p.prg \
	vpush32.prg \
	vpush32p.prg \
	writerec.prg \
	xor16.prg \
	xor32.prg \
	zero32.prg


.SUFFIXES: .asm .prg

$(PROJECT): $(OBJS)
	cat fenter.prg > $(PROJECT)
	cat fdata.prg >> $(PROJECT)
	cat ffwrite.prg >> $(PROJECT)
	cat ffread.prg >> $(PROJECT)
	cat uwrite.prg >> $(PROJECT)
	cat uread.prg >> $(PROJECT)
	cat fmtread.prg >> $(PROJECT)
	cat fmtwrite.prg >> $(PROJECT)
	cat writerec.prg >> $(PROJECT)
	cat fromsci.prg >> $(PROJECT)
	cat round45.prg >> $(PROJECT)
	cat fwrite.prg >> $(PROJECT)
	cat fread.prg >> $(PROJECT)
	cat fopen.prg >> $(PROJECT)
	cat fclose.prg >> $(PROJECT)
	cat frewind.prg >> $(PROJECT)
	cat ferrno.prg >> $(PROJECT)
	cat ferrao.prg >> $(PROJECT)
	cat ioflag.prg >> $(PROJECT)
	cat fpos.prg >> $(PROJECT)
	cat fseek.prg >> $(PROJECT)
	cat iostatus.prg >> $(PROJECT)
	cat fildes.prg >> $(PROJECT)
	cat mul16.prg >> $(PROJECT)
	cat rnd16.prg >> $(PROJECT)
	cat mod16.prg >> $(PROJECT)
	cat div16.prg >> $(PROJECT)
	cat mdnorm16.prg >> $(PROJECT)
	cat add16.prg >> $(PROJECT)
	cat eq16.prg >> $(PROJECT)
	cat ne16.prg >> $(PROJECT)
	cat gt16.prg >> $(PROJECT)
	cat gte16.prg >> $(PROJECT)
	cat lt16.prg >> $(PROJECT)
	cat lte16.prg >> $(PROJECT)
	cat sub16.prg >> $(PROJECT)
	cat and16.prg >> $(PROJECT)
	cat or16.prg >> $(PROJECT)
	cat xor16.prg >> $(PROJECT)
	cat true16.prg >> $(PROJECT)
	cat false16.prg >> $(PROJECT)
	cat abs16.prg >> $(PROJECT)
	cat sgn16.prg >> $(PROJECT)
	cat nextvar.prg >> $(PROJECT)
	cat next.prg >> $(PROJECT)
	cat atoi16.prg >> $(PROJECT)
	cat itoa16.prg >> $(PROJECT)
	cat tobcd16.prg >> $(PROJECT)
	cat alloc.prg >> $(PROJECT)
	cat dealloc.prg >> $(PROJECT)
	cat hfree.prg >> $(PROJECT)
	cat checkeom.prg >> $(PROJECT)
	cat fptan.prg >> $(PROJECT)
	cat fpcos.prg >> $(PROJECT)
	cat fpsin.prg >> $(PROJECT)
	cat fppow.prg >> $(PROJECT)
	cat fpln.prg >> $(PROJECT)
	cat fpexp.prg >> $(PROJECT)
	cat fpasin.prg >> $(PROJECT)
	cat fpacos.prg >> $(PROJECT)
	cat fpatan.prg >> $(PROJECT)
	cat fpsqrt.prg >> $(PROJECT)
	cat fact.prg >> $(PROJECT)
	cat addtows.prg >> $(PROJECT)
	cat fpcopy.prg >> $(PROJECT)
	cat getargs.prg >> $(PROJECT)
	cat atof.prg >> $(PROJECT)
	cat ftoa.prg >> $(PROJECT)
	cat mulfp.prg >> $(PROJECT)
	cat mulfpi.prg >> $(PROJECT)
	cat divfp.prg >> $(PROJECT)
	cat divfpi.prg >> $(PROJECT)
	cat absfp.prg >> $(PROJECT)
	cat sgnfp.prg >> $(PROJECT)
	cat eqfp.prg >> $(PROJECT)
	cat nefp.prg >> $(PROJECT)
	cat gtfp.prg >> $(PROJECT)
	cat gtefp.prg >> $(PROJECT)
	cat ltfp.prg >> $(PROJECT)
	cat ltefp.prg >> $(PROJECT)
	cat subfp.prg >> $(PROJECT)
	cat subfpi.prg >> $(PROJECT)
	cat addfp.prg >> $(PROJECT)
	cat addfpi.prg >> $(PROJECT)
	cat fpdata.prg >> $(PROJECT)
	cat fpnorm.prg >> $(PROJECT)
	cat fpargs.prg >> $(PROJECT)
	cat fpret0.prg >> $(PROJECT)
	cat fpreta.prg >> $(PROJECT)
	cat fpretb.prg >> $(PROJECT)
	cat fpcomp2.prg >> $(PROJECT)
	cat itof.prg >> $(PROJECT)
	cat ftoi.prg >> $(PROJECT)
	cat rnd32.prg >> $(PROJECT)
	cat mul32.prg >> $(PROJECT)
	cat mul32i.prg >> $(PROJECT)
	cat add32.prg >> $(PROJECT)
	cat add32i.prg >> $(PROJECT)
	cat dec32.prg >> $(PROJECT)
	cat mod32.prg >> $(PROJECT)
	cat div32.prg >> $(PROJECT)
	cat div32i.prg >> $(PROJECT)
	cat sub32.prg >> $(PROJECT)
	cat sub32i.prg >> $(PROJECT)
	cat neg32.prg >> $(PROJECT)
	cat icomp32.prg >> $(PROJECT)
	cat shl32.prg >> $(PROJECT)
	cat shr32.prg >> $(PROJECT)
	cat zero32.prg >> $(PROJECT)
	cat lfsrshift.prg >> $(PROJECT)
	cat null32.prg >> $(PROJECT)
	cat and32.prg >> $(PROJECT)
	cat or32.prg >> $(PROJECT)
	cat xor32.prg >> $(PROJECT)
	cat eq32.prg >> $(PROJECT)
	cat ne32.prg >> $(PROJECT)
	cat lt32.prg >> $(PROJECT)
	cat lte32.prg >> $(PROJECT)
	cat gt32.prg >> $(PROJECT)
	cat gte32.prg >> $(PROJECT)
	cat cmp32.prg >> $(PROJECT)
	cat comp32.prg >> $(PROJECT)
	cat true32.prg >> $(PROJECT)
	cat false32.prg >> $(PROJECT)
	cat abs32.prg >> $(PROJECT)
	cat sgn32.prg >> $(PROJECT)
	cat tobcd32.prg >> $(PROJECT)
	cat itoa32.prg >> $(PROJECT)
	cat atoi32.prg >> $(PROJECT)
	cat nextvar32.prg >> $(PROJECT)
	cat next32.prg >> $(PROJECT)
	cat strcat.prg >> $(PROJECT)
	cat right.prg >> $(PROJECT)
	cat strcpy.prg >> $(PROJECT)
	cat strcpyin.prg >> $(PROJECT)
	cat strlen.prg >> $(PROJECT)
	cat mid.prg >> $(PROJECT)
	cat left.prg >> $(PROJECT)
	cat tolower.prg >> $(PROJECT)
	cat toupper.prg >> $(PROJECT)
	cat strcmp.prg >> $(PROJECT)
	cat checkeof.prg >> $(PROJECT)
	cat isnumeral.prg >> $(PROJECT)
	cat ioresults16.prg >> $(PROJECT)
	cat ioresults32.prg >> $(PROJECT)
	cat vpush8p.prg >> $(PROJECT)
	cat vpush16p.prg >> $(PROJECT)
	cat vpush32.prg >> $(PROJECT)
	cat vpush32p.prg >> $(PROJECT)
	cat epush.prg >> $(PROJECT)
	cat scratch.prg >> $(PROJECT)
	cat lfsr.prg >> $(PROJECT)
	cat heap.prg >> $(PROJECT)
	cat free.prg >> $(PROJECT)

.asm.prg:
	asm02 -l -L $<

clean:
	-rm *.prg
	-rm $(PROJECT)

