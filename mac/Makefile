SRV = main.c
CLIENT = client.m
SRV_GPT = gpt_srv.c
CLIENT_GPT = gpt_client.c
BIN = bin

all: $(BIN)
	clang -Wno-deprecated-declarations -g3 $(SRV) -o $(BIN)/server -O1
	clang -framework Foundation -framework CoreGraphics -Wno-deprecated-declarations -g3 $(CLIENT) -o $(BIN)/client -O1

a:
	make -C gpt/

$(BIN):
	mkdir $(BIN)

clean:
	@rmdir -rf $(BIN)
	@echo Cleaned bin folder

.PHONY: a clean all
