
.PHONY : all clean

all : api.png builder.png configuration.png core.png full.png socket.png thread.png

clean :
	@rm -vf api.png builder.png configuration.png core.png full.png socket.png thread.png

api.png : api.dot
		dot -T png -o api.png api.dot

builder.png : builder.dot
		dot -T png -o builder.png builder.dot

configuration.png : configuration.dot
		dot -T png -o configuration.png configuration.dot

core.png : core.dot
		dot -T png -o core.png core.dot

full.png : full.dot
		dot -T png -o full.png full.dot

socket.png : socket.dot
		dot -T png -o socket.png socket.dot

thread.png : thread.dot
		dot -T png -o thread.png thread.dot

