all: example_wrap example.so
PY3_INCLUDES=-I/usr/include/python3.4m -I/usr/include/python3.4m
PY3_LIBS=-lpython3.4m -lpthread -ldl  -lutil -lm 


# Use $(python3-config --includes) to get PY3_INCLUES


example_wrap: example.swig example.h example.cpp
	swig -c++ -python -py3 example.swig


example.so: example_wrap
	g++ -fPIC -c example.cpp
	g++ $(PY3_INCLUDES) -fPIC -c example_wrap.cxx
	g++ -shared example_wrap.o example.o $(PY3_LIBS) -o example.so 



clean:
	rm example_wrap.cxx example.py
	rm *.o
	rm *.so
