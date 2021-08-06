#ifndef H_GENERIC_LIST
#define H_GENERIC_LIST

/* types =============================================================== */

typedef struct _genericList GenericList;

/* entry points ======================================================== */

GenericList *GenericListCreate();
void GenericListFree(GenericList *list);
void GenericCellAdd(GenericList *list, void *info);
void *GenericCellGet(GenericList *list, int number);
void GenericCellFree(GenericList *list, int number);
int GenericCellGetNumber(GenericList *list, void *info);
int GenericListArraySize(GenericList *list);

#endif