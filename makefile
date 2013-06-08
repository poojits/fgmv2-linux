#Made by Poojit.
CFLAGS = -g -O3 -fPIC
LFLAGS = -lm
BLAS = blas/blas.a
CC ?=gcc
outdir = ./build
srcdir = ./src
includedir = ./include
# All Target

all: predict train

FGM.o: $(srcdir)/FGM.cpp
	$(CXX) $(CFLAGS) -I $(includedir) -c $(srcdir)/FGM.cpp -o $(outdir)/FGM.o $(LFLAGS)

MKL.o: $(srcdir)/MKL.cpp
	$(CXX) $(CFLAGS) -I $(includedir) -c $(srcdir)/MKL.cpp -o $(outdir)/MKL.o $(LFLAGS)

ProxMKL.o: $(srcdir)/ProxMKL.cpp
	$(CXX) $(CFLAGS) -I $(includedir) -c $(srcdir)/ProxMKL.cpp -o $(outdir)/ProxMKL.o $(LFLAGS)
	
ProxOMP.o: $(srcdir)/ProxOMP.cpp
	$(CXX) $(CFLAGS) -I $(includedir) -c $(srcdir)/ProxOMP.cpp -o $(outdir)/ProxOMP.o $(LFLAGS)

stocha_prox_mkl.o: $(srcdir)/stocha_prox_mkl.cpp
	$(CXX) $(CFLAGS) -I $(includedir) -c $(srcdir)/stocha_prox_mkl.cpp -o $(outdir)/stocha_prox_mkl.o $(LFLAGS)

linear.o: $(srcdir)/linear.cpp
	$(CXX) $(CFLAGS) -I $(includedir) -c $(srcdir)/linear.cpp -o $(outdir)/linear.o $(LFLAGS)

tron.o: $(srcdir)/tron.cpp
	$(CXX) $(CFLAGS) -I $(includedir) -c $(srcdir)/tron.cpp -o $(outdir)/tron.o $(LFLAGS)

random.o: $(srcdir)/random.cpp
	$(CXX) $(CFLAGS) -I $(includedir) -c $(srcdir)/random.cpp -o $(outdir)/random.o $(LFLAGS)
	
train: $(srcdir)/train.cpp linear.o stocha_prox_mkl.o ProxMKL.o MKL.o FGM.o tron.o random.o blas/blas.a ProxOMP.o
	$(CXX) $(CFLAGS) -I $(includedir) $(srcdir)/train.cpp $(outdir)/linear.o $(outdir)/stocha_prox_mkl.o $(outdir)/ProxMKL.o $(outdir)/tron.o $(outdir)/random.o $(outdir)/MKL.o $(outdir)/FGM.o $(outdir)/ProxOMP.o -o train $(LFLAGS) $(BLAS)

predict: $(srcdir)/predict.cpp linear.o stocha_prox_mkl.o ProxMKL.o MKL.o FGM.o random.o tron.o blas/blas.a ProxOMP.o
	$(CXX) $(CFLAGS) -I $(includedir) $(srcdir)/predict.cpp $(outdir)/linear.o $(outdir)/stocha_prox_mkl.o $(outdir)/ProxMKL.o $(outdir)/MKL.o $(outdir)/FGM.o $(outdir)/tron.o $(outdir)/random.o $(outdir)/ProxOMP.o -o predict $(LFLAGS) $(BLAS)

blas/blas.a: blas/*.c blas/*.h
	make -C blas OPTFLAGS='$(CFLAGS)' CC='$(CC)';

clean:
	rm -rf *.o predict train $(outdir)/*.o
	make -C blas clean
