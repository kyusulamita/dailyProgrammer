#include<iostream>
#include<string>


using namespace std;
//Requires: a string, width >= 1 height >= 1
//Modies:Nothing
//Effects:makes a triangle from the word shitPost
void shitPostCreator(string shitPost, int width, int height);

void shitPostCreator(string shitPost, int width, int height){
	int len_min1 = shitPost.length() - 1;
	int doubleLen = len_min1 * 2;
	int tWidth = (width == 1) ? len_min1 + 1: ((width * (len_min1)) + 1);
	int tHeight = (height == 1) ? len_min1  + 1 : ((height * (len_min1)) + 1);
	string triBase = shitPost;
	for( int i = len_min1 - 1; i > 0; i--) triBase += shitPost[i];

	for (int curRow = 0; curRow < tHeight; curRow++){
		for (int curCol = 0; curCol < tWidth; curCol++){
			if (!(curRow % doubleLen)) cout << triBase[curCol % doubleLen];
			else if (!(curRow % len_min1)) cout << triBase[(curCol + len_min1) % doubleLen]; 
			else if (!(curCol % doubleLen)) cout << triBase[curRow % doubleLen];
			else if (!(curCol % len_min1)) cout << triBase[(curRow + len_min1) % doubleLen];
			else cout << ' ';
			cout << ' ';
		}
		cout << endl;
	}
	cout << endl << endl;
}


int main(){
	shitPostCreator("rekt" , 2 , 2);
	shitPostCreator("rekt" , 4 , 3);
	shitPostCreator("maurica", 2, 2);
}