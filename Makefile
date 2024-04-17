SRV = serv.c
CLIENT = client.c
BIN = bin

all: $(BIN)
	clang -Wno-deprecated-declarations -g3 $(SRV) -o $(BIN)/$(SRV).exe -O1 -lws2_32
	clang -Wno-deprecated-declarations -g3 $(CLIENT) -o $(BIN)/$(CLIENT).exe -O1 -lws2_32

$(BIN):
	mkdir $(BIN)

clean:
	@rmdir $(BIN) /S /Q
	@echo Cleaned bin folder
