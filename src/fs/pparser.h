#ifndef PATHPASHER_H
#define PATHPASHER_H

/* 
    Define a Structure which Consist the Path
    Example : 1/test/text.txt
    1 -> Drive No
    test -> First Path
    text.txt -> part/next path
*/
struct path_root
{
    int drive_no;
    struct path_part *first;
};

struct path_part
{
    const char *part;
    struct path_part *next;
};

struct path_root *pathparser_parse(const char *path,const char *current_directory_path);
void pathparser_free(struct path_root *root);


#endif