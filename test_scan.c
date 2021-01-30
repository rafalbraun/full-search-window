#include "scan_file.c"

int main () {
	//scan_file ("scan_file.c", "[a-z]{3}", NULL);
	scan_file ("scan_file.c", "the ((red|white) (king|queen))", NULL);


}