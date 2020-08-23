#include "parallelcopy.h"
#include <iostream.h>

void parllCopy::copyData()
{
    cout << source_tabTag << "\t" << dest_StartRow << endl;
    cout << dest_tabTag << "\t" << source_StartRow << endl;
    
    for(int i = 0; i < source_numRow; ++i)
    {
	for(int j = 0; j < source_numCol; ++j)
	{
	    cout << "\nA: " << i + dest_StartRow << endl;
	    cout << "\nB: " << j + dest_StartCol << endl;
	    cout << "\nC: " << i + source_StartRow << endl;
	    cout << "\nD: " << j + source_StartCol << endl;
	    
	    dest_tabTag->setText(i + dest_StartRow, j + dest_StartCol, source_tabTag->text(i + source_StartRow, j + source_StartCol));
	}
    }
}
