output: client.cpp newserver.cpp
	g++ -o newserver newserver.cpp -lSDL2 -lSDL2_net
	g++ -o client client.cpp -lSDL2 -lSDL2_net	

.PHONY: clean 
clean:
	rm newserver
	rm client