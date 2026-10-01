# Build & run any solution (run from Practice/bclx/):
#   make build PROB=03_sum_of_first_n_natural SRC=result/v2_allreduce.cpp
#   make run   PROB=03_sum_of_first_n_natural SRC=result/v2_allreduce.cpp NP=4 ARGS="1000000"
#   make run   PROB=09_prime_number SRC=mine/mine.cpp NP=2 ARGS="1000000007"

PROB ?= 01_positive_or_negative
SRC  ?= result/v1_basic.cpp
NP   ?= 4
ARGS ?=

DIR_BCL  = ../../bcl
DIR_BCLX = ../../bclx
FLAGS    = -std=gnu++17 -O2 -Wall -I../..

OUT = Problems/$(PROB)/$(basename $(SRC)).out

.PHONY : build run clean

build:
	mpic++ Problems/$(PROB)/$(SRC) -o $(OUT) $(FLAGS) -I$(DIR_BCL) -I$(DIR_BCLX)

# OMPI_MCA_osc=pt2pt works around an Open MPI RMA issue (see repo root README.md)
run: build
	export OMPI_MCA_osc=pt2pt; \
	mpirun -np $(NP) $(OUT) $(ARGS)

clean:
	find Problems -name '*.out' -delete
