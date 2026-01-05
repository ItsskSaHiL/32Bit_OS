#include "pparser.h"
#include "string/string.h"
#include "kernel.h"
#include "memory/heap/kheap.h"
#include "status.h"
#include "memory/memory.h"

static int pathparser_path_valid_format(const char *filename)
{
    int len = strnlen(filename, MYOS_MAX_PATHLEN);
    return (len >= 3 && isDigit(filename[0]) && memcmp((void*)&filename[1],":/",2) == 0); 
}

static int pathparser_get_drive_by_path(const char **path)
{
    if(!pathparser_path_valid_format(*path))
    {
        return -EBADPATH;
    }

    int drive_no = tonumericdigit(*path[0]);

    // Add 3 for skip the drive no like 0:/ , 1:/ 
    // like 0:/path/path.txt -> path/path.txt
    *path += 3;
    return drive_no;
}

/*
    Create Root Path
*/
static struct path_root *pathparser_create_root(int drive_number)
{
    struct path_root *path_r = kzalloc(sizeof(struct path_root));
    path_r->drive_no = drive_number;
    path_r->first = 0;
    return path_r;
}

/*
    Get Path 
*/
static const char *pathparser_get_path_part(const char **path)
{
    char *result_path_part = kzalloc(MYOS_MAX_PATHLEN);
    int i = 0;
    while(**path != '/' && **path != 0x00)
    {
        result_path_part[i] = **path;
        *path += 1;
        i++;
    }

    if(**path == '/')
    {
        // Skip the '/'
        *path += 1;
    }

    if(i == 0)
    {
        kfree(result_path_part);
        result_path_part = 0;
    }
    return result_path_part;
}

struct path_part* pathparser_parser_path_part(struct path_part *last_part,const char **path)
{
    const char *path_part_str = pathparser_get_path_part(path);
    if(path_part_str == 0)
    {
        return 0;
    }
    struct path_part *part = kzalloc(sizeof(struct path_part));
    part->part = path_part_str;
    part->next = 0x00;

    if(last_part)
    {
        last_part->next = part;
    }
    return part;
}

void pathparser_free(struct path_root *root)
{
    struct path_part *part = root->first;
    while(part)
    {
        struct path_part *next_part = part->next;
        kfree((void*)part->part);
        kfree(part);
        part = next_part;
    }
    kfree(root); 
}

/*
    Main Function for allocate Path
*/
struct path_root *pathparser_parse(const char *path,const char *current_directory_path)
{
    int res = 0;
    const char *temp_path = path;
    struct path_root *path_root = 0;
    
    // validate pathlen
    if(strlen(path) > MYOS_MAX_PATHLEN)
    {
        goto out;
    }

    // get the drive 0:/ or 1:/ -> 0 or 1
    res = pathparser_get_drive_by_path(&temp_path);
    if(res < 0)
    {
        goto out;
    }

    // Create Root file
    path_root = pathparser_create_root(res);
    if(!path_root)
    {
        goto out;
    }

    // Get First Path
    struct path_part *first_part = pathparser_parser_path_part(NULL,&temp_path);
    if(!first_part)
    {
        goto out;
    }

    // if there are path exist then add them with root
    path_root->first = first_part;

    /* 
    Itterate though temp_path and add with first_part */
    struct path_part *part = pathparser_parser_path_part(first_part,&temp_path);
    while(part)
    {
        part = pathparser_parser_path_part(part,&temp_path);
    }

out:
    return path_root;
}