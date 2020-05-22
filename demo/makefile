all: demo

run: demo
	export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/systemc/lib-linux64/ && ./demo

demo:
	g++ -o demo -I /usr/local/systemc/include/ -L /usr/local/systemc/lib-linux64/ -lsystemc -lm -DSC_ENABLE_PROFILER demo.cpp

show: run
	python ../viewer/scviewer.py profile.json

clean:
	rm profile.json demo
