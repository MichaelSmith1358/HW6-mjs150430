/*
 * Usage of CDK Matrix
 *
 * Name:   Michael Smith
 * Class:  CS3377.501
 * Email:  mjs150430@utdallas.edu
 */

#include <iostream>
#include "cdk.h"
#include <iomanip>
#include <fstream>
#include <cstdint>
#include <string>
#include <sstream>
#include <cstring>
#include <stdint.h>

#define MATRIX_WIDTH 5
#define MATRIX_HEIGHT 3
#define BOX_WIDTH 20
#define MATRIX_NAME_STRING "Test Matrix"

using namespace std;

class BinaryFileHeader {
public:

  uint32_t magicNumber;
  uint32_t versionNumber;
  uint64_t numRecords;
};

const int maxRecordStringLength = 25;

class BinaryFileRecord{
public:

  uint8_t strLength;
  char stringBuffer[maxRecordStringLength];
};

int main()
{

  WINDOW	*window;
  CDKSCREEN	*cdkscreen;
  CDKMATRIX     *myMatrix;           // CDK Screen Matrix

  // Remember that matrix starts out at 1,1.
  // Since arrays start out at 0, the first entries
  // below ("R0", and "C0") are just placeholders
  // 
  // Finally... make sure your arrays have enough entries given the
  // values you choose to set for MATRIX_WIDTH and MATRIX_HEIGHT
  // above.

  const char 		*rowTitles[] = {"R0", "a", "b", "c", "d", "e"};
  const char 		*columnTitles[] = {"C0", "a", "b", "c", "d", "e"};
  int		boxWidths[] = {BOX_WIDTH, BOX_WIDTH, BOX_WIDTH, BOX_WIDTH, BOX_WIDTH, BOX_WIDTH};
  int		boxTypes[] = {vMIXED, vMIXED, vMIXED, vMIXED,  vMIXED,  vMIXED};

  /*
   * Initialize the Cdk screen.
   *
   * Make sure the putty terminal is large enough
   */
  window = initscr();
  cdkscreen = initCDKScreen(window);

  /* Start CDK Colors */
  initCDKColor();

  /*
   * Create the matrix.  Need to manually cast (const char**) to (char **)
  */
  myMatrix = newCDKMatrix(cdkscreen, CENTER, CENTER, MATRIX_WIDTH, MATRIX_HEIGHT, MATRIX_WIDTH, MATRIX_HEIGHT,
			  MATRIX_NAME_STRING, (char **) rowTitles, (char **) columnTitles, boxWidths,
				     boxTypes, 1, 1, ' ', ROW, true, true, false);
  
  BinaryFileHeader *binHead = new BinaryFileHeader();

  BinaryFileRecord *binRec = new BinaryFileRecord();

  ifstream binInFile ("cs3377.bin", ios::in | ios::binary);
  binInFile.read((char *) binHead, sizeof(BinaryFileHeader));
  
  if (myMatrix ==NULL)
    {
      printf("Error creating Matrix\n");
      _exit(1);
    }

  /* Display the Matrix */
  drawCDKMatrix(myMatrix, true);
  
  stringstream stream;
  stream << uppercase << hex << binHead->magicNumber;
  string result(stream.str());
  setCDKMatrixCell(myMatrix,1,1,string("Magic: "+ string("0x")+ string(result.c_str())).c_str());
  drawCDKMatrix(myMatrix, true);
  stream.str("");
  stream << dec << binHead->versionNumber;
  result = stream.str();
  setCDKMatrixCell(myMatrix,1,2,string("Version: "+ string(result.c_str())).c_str());
  stream.str("");
  stream << dec << binHead->numRecords;
  result = stream.str();
  setCDKMatrixCell(myMatrix,1,3,string("NumRecords: "+ string(result.c_str())).c_str());
  
  for(int z = 2; z<6; z++){
    binInFile.read((char *) binRec, sizeof(BinaryFileRecord));
    if(binInFile){
	result = to_string(binRec->strLength);
	setCDKMatrixCell(myMatrix,z,1,string("strlen: "+ result).c_str());
	
	result = binRec->stringBuffer;
	setCDKMatrixCell(myMatrix,z,2,result.c_str());
	
	continue;
    }
    else{
      z = 6;
      continue;
    }
  
  }
  /*
   * Dipslay a message
   */
  //setCDKMatrixCell(myMatrix, 2, 2, "Test Message");
  drawCDKMatrix(myMatrix, true);    /* required  */

  /* So we can see results, pause until a key is pressed. */
  unsigned char x;
  cin >> x;

  // Cleanup screen
  endCDK();
}
