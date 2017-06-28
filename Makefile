BUILD_DIR=./build

all:
	mkdir $(BUILD_DIR)
	cd $(BUILD_DIR); cmake ..; make

clean:
	rm $(BUILD_DIR) -rf
