/* C program for Boyer Moore Algorithm with 
   Good Suffix heuristic to find pattern in
   given text string */
#include <bits/stdc++.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include<fstream>
using namespace std;

# define NO_OF_CHARS 256
long length = 0;
  
// preprocessing for strong good suffix rule
void preprocess_strong_suffix(long *shift, long *bpos,
                                char *pat, long m)
{
    // m is the length of pattern 
    long i=m, j=m+1;
    bpos[i]=j;
  
    while(i>0)
    {
        /*if character at position i-1 is not equivalent to
          character at j-1, then continue searching to right
          of the pattern for border */
        while(j<=m && pat[i-1] != pat[j-1])
        {
            /* the character preceding the occurrence of t in 
               pattern P is different than the mismatching character in P, 
               we stop skipping the occurrences and shift the pattern
               from i to j */
            if (shift[j]==0)
                shift[j] = j-i;
  
            //Update the position of next border 
            j = bpos[j];
        }
        /* p[i-1] matched with p[j-1], border is found.
           store the  beginning position of border */
        i--;j--;
        bpos[i] = j; 
    }
}
  
//Preprocessing for case 2
void preprocess_case2(long *shift, long *bpos,
                      char *pat, long m)
{
    long i, j;
    j = bpos[0];
    for(i=0; i<=m; i++)
    {
        /* set the border position of the first character of the pattern
           to all indices in array shift having shift[i] = 0 */ 
        if(shift[i]==0)
            shift[i] = j;
  
        /* suffix becomes shorter than bpos[0], use the position of 
           next widest border as value of j */
        if (i==j)
            j = bpos[j];
    }
}
  
// The preprocessing function for Boyer Moore's
// bad character heuristic
void badCharHeuristic( char *str, long size,
                        long badchar[NO_OF_CHARS])
{
    int i;
 
    // Initialize all occurrences as -1
    for (i = 0; i < NO_OF_CHARS; i++)
        badchar[i] = -1;
 
    // Fill the actual value of last occurrence
    // of a character
    for (i = 0; i < size; i++)
        badchar[(int) str[i]] = i;
}

/*Search for a pattern in given text using
  Boyer Moore algorithm with Good suffix rule */
void search(char *text, char *pat)
{
    // s is shift of the pattern with respect to text
    long s=0, j;
    long m = strlen(pat);
    long n = strlen(text);
    cout << "M " << m << " " << "N " << n << endl;
    long badchar[NO_OF_CHARS];
    long bpos[m+1], shift[m+1];
  
    //initialize all occurrence of shift to 0
    for(long i=0;i<m+1;i++) {
        shift[i]=0;
    } 
  
    //do preprocessing
    /* Fill the bad character array by calling
    the preprocessing function badCharHeuristic()
    for given pattern */
    badCharHeuristic(pat, m, badchar);
    preprocess_strong_suffix(shift, bpos, pat, m);
    preprocess_case2(shift, bpos, pat, m);
    
    long z = 1;
    while(s <= n-m)
    {
  
        j = m-1;
  
        /* Keep reducing index j of pattern while characters of
             pattern and text are matching at this shift s*/
        while(j >= 0 && pat[j] == text[s+j])
            j--;
  
        /* If the pattern is present at the current shift, then index j
             will become -1 after the above loop */
        if (j<0)
        {
            printf("pattern occurs at shift = %ld\n", s);
            s += shift[0];
        }
        else
            /*pat[i] != pat[s+j] so shift the pattern
              shift[j+1] times  */
            // s += shift[j+1];
            s += max(z, max(j - badchar[text[s + j]], shift[j+1]));
            // cout << "Values: " << 1 << " " << j - badchar[text[s + j]] << " " << shift[j+1] << "\n";
            // cout << "Max: " << max(1, max(j - badchar[text[s + j]], shift[j+1])) << "\n";
    }
    cout << pat[0] << " " << text[0];
}

char* readInput(string fname) {
    const char* fname2 = fname.c_str();
    FILE *f = fopen(fname2, "r");
    char* buffer;

    if (f) {
        fseek(f, 0, SEEK_END);
        length = ftell(f);
        fseek(f, 0, SEEK_SET);
        buffer = (char *)malloc( sizeof(char) * ( length + 1 ) );
        
        if(buffer) {
            fread(buffer, sizeof(char), length, f);
        }
        fclose(f);
    }
    buffer[length] = '\0';
    cout << "The length of the text is " << length << endl;
    return buffer;
}

//Driver 
int main()
{
    // char text[] = "ABAAAABAACD";
    char pat[] = "CCCCCCCCCCCCCCCCCCCCCCCCCCC";
    string fname = "output.txt";

    // Reading in file and calculating length
    cout << endl << "Reading in file ..." << endl;
    char *text = readInput(fname);
    cout << endl << "Successfully Read in File!" << endl;

    // Loops for extracting sequences
    long offset = 10000;
    int i = 0;
    cout << endl <<"START STRING" << endl;
    while (i < 10) {
        printf("%c", text[offset + i]);
        i++;
    }
    i = 0;
    cout << endl << "MIDDLE STRING" << endl;
    while (i < 10) {
        printf("%c", text[(long)(length/2) + i]);
        i++;
    }
    i = 0;
    cout << endl << "END STRING" << endl;
    while (i < 10) {
        printf("%c", text[length-10 + i]);
        i++;
    }
    cout << endl << endl;
    // Call Search Function
    search(text, pat);
    cout << endl << "Search completed!" << endl;
    return 0;
}