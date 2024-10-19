default:
	# write command to test your files (you do not need it for your submissions)
	g++ -std=c++11 main.cpp -o main
	g++ -std=c++11 q2.cpp -o q2 -I/opt/homebrew/opt/openssl/include -L/opt/homebrew/opt/openssl/lib -lssl -lcrypto
	
clean:
	rm *.out
