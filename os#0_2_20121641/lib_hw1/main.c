#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "list.h"
#include "hash.h"
#include "bitmap.h"
#define BUFFMAX 100
#define TOKLENMAX 30
#define TOKMAX 10
#define LISTMAX 10
#define HASHMAX 10
#define BITMAPMAX 10

#define list_elem_to_hash_elem(LIST_ELEM)                       \
        list_entry(LIST_ELEM, struct hash_elem, list_elem)



struct namedList
{
		struct list *thisList;
		char name[TOKLENMAX]; 
};
struct namedHash
{
		struct hash *thisHash;
		char name[TOKLENMAX];
};
struct namedBitmap
{
		struct bitmap *thisBitmap;
		char name[TOKLENMAX];
};

struct list_item
{
		struct list_elem elem;
		int data;
};
struct hash_item
{
		struct hash_elem elem;
		int data;
};//// ???????

struct bitmap *bitmap_expand(struct bitmap * bitmap, int size);


void callCreH(char toks[TOKMAX][TOKLENMAX]);
bool hash_data_less(const struct hash_elem *a, const struct hash_elem *b, void *aux);
void call_hash_insert(char toks[TOKMAX][TOKLENMAX]);
int getNamedHashIdx(char* name);
void call_hash_replace(char toks[TOKMAX][TOKLENMAX]);
void call_hash_find(char toks[TOKMAX][TOKLENMAX]);
void call_hash_delete(char toks[TOKMAX][TOKLENMAX]);
void call_hash_clear(char toks[TOKMAX][TOKLENMAX]);
void call_hash_size(char toks[TOKMAX][TOKLENMAX]);
void call_hash_empty(char toks[TOKMAX][TOKLENMAX]);
void call_hash_apply(char toks[TOKMAX][TOKLENMAX]);
void hash_destructor(struct hash_elem *a, void *aux);
void square(struct hash_elem *e, void *aux);
void triple(struct hash_elem *e, void *aux);

void callCreB(char toks[TOKMAX][TOKLENMAX]);
void call_bitmap_mark(char toks[TOKMAX][TOKLENMAX]);
void call_bitmap_size(char toks[TOKMAX][TOKLENMAX]);
void call_bitmap_set(char toks[TOKMAX][TOKLENMAX]);
void call_bitmap_reset(char toks[TOKMAX][TOKLENMAX]);
void call_bitmap_flip(char toks[TOKMAX][TOKLENMAX]);
void call_bitmap_test(char toks[TOKMAX][TOKLENMAX]);
void call_bitmap_set_all(char toks[TOKMAX][TOKLENMAX]);
void call_bitmap_set_multiple(char toks[TOKMAX][TOKLENMAX]);
void call_bitmap_count(char toks[TOKMAX][TOKLENMAX]);
void call_bitmap_contains(char toks[TOKMAX][TOKLENMAX]);
void call_bitmap_any(char toks[TOKMAX][TOKLENMAX]);
void call_bitmap_none(char toks[TOKMAX][TOKLENMAX]);
void call_bitmap_all(char toks[TOKMAX][TOKLENMAX]);
void call_bitmap_scan(char toks[TOKMAX][TOKLENMAX]);
void call_bitmap_scan_and_flip(char toks[TOKMAX][TOKLENMAX]);
void call_bitmap_dump(char toks[TOKMAX][TOKLENMAX]);
void call_bitmap_expand(char toks[TOKMAX][TOKLENMAX]);
int getNamedBitmapIdx(char* name);


bool callPinFun( char toks[TOKMAX][TOKLENMAX] );
int getCmdIdx(char*);
struct list_elem*  getElem_idx(int listIdx, int pos);
int getNamedListIdx(char* name);
void call_list_swap(char toks[TOKMAX][TOKLENMAX]);
void call_list_shuffle(char toks[TOKMAX][TOKLENMAX]);
bool call_list_splice(char toks[TOKMAX][TOKLENMAX] );
bool call_list_remove(char toks[TOKMAX][TOKLENMAX] );
struct list_elem * call_list_pop_front(char toks[TOKMAX][TOKLENMAX]);
struct list_elem * call_list_pop_back(char toks[TOKMAX][TOKLENMAX]);
struct list_elem * call_list_front(char toks[TOKMAX][TOKLENMAX]);
struct list_elem * call_list_back(char toks[TOKMAX][TOKLENMAX]);
int call_list_size(char toks[TOKMAX][TOKLENMAX]);
bool call_list_empty(char toks[TOKMAX][TOKLENMAX]);
int call_list_max(char toks[TOKMAX][TOKLENMAX]);
bool list_itemData_less(const struct list_elem * elem1, const struct list_elem * elem2, void* aux);
int call_list_min(char toks[TOKMAX][TOKLENMAX]);
bool call_list_reverse(char toks[TOKMAX][TOKLENMAX]);
bool call_list_sort(char toks[TOKMAX][TOKLENMAX]);

bool call_list_unique(char toks[TOKMAX][TOKLENMAX]);
bool call_list_insert_ordered(char toks[TOKMAX][TOKLENMAX]);

bool callCre( char toks[TOKMAX][TOKLENMAX] );
void callDel( char toks[TOKMAX][TOKLENMAX] );
bool callDum( char toks[TOKMAX][TOKLENMAX] );
void callCom( char toks[TOKMAX][TOKLENMAX] );
bool callCreL( char toks[TOKMAX][TOKLENMAX] );
void makeToken(char buff[BUFFMAX], char toks[TOKMAX][TOKLENMAX] );

struct namedList namedListArr[LISTMAX];
struct namedHash namedHashArr[HASHMAX];
struct namedBitmap namedBitmapArr[BITMAPMAX];
int namedListCnt = -1;
int namedHashCnt = -1;
int namedBitmapCnt = -1;

int main()
{
		char cmdBuff[BUFFMAX] = {0};
		char cmdTok[TOKMAX][TOKLENMAX];
	     ////////temp
		////////

		while(1)
		{
//			printf(">");
			fgets(cmdBuff, BUFFMAX, stdin);

			if(!strcmp(cmdBuff,"\n")) continue;
			makeToken(cmdBuff, cmdTok);
			if(!strcmp("quit",cmdTok[0]))
					break;
			else if(!strcmp("create",cmdTok[0]))
			{
					callCre(cmdTok);
			}
			else if(!strcmp("delete",cmdTok[0]))
					callDel(cmdTok);
			else if(!strcmp("dumpdata",cmdTok[0]))
			{		
					callDum(cmdTok);
			}
		/*	else if(!strcmp("command",cmdTok[0]))
					callCom();
			
		*/	
			else
			{
					callPinFun(cmdTok);
			}
			////////temp
//			printf("is success : %d", isScs);
			
/*			for(i = 0;i<10;i++)
					
							
							printf("%s \n",cmdTok[i]);
*/					
			//////
		}

		return 0;
}

/*struct bitmap *bitmap_expand(struct bitmap * bm, int size)
{


	bm->bits = realloc(bm->bits, byte_cnt(size));
	bm->bit_cnt = bm->bit_cnt + size;

}*/
unsigned hash_func(const struct hash_elem *e, void *aux)
{
		int data = hash_entry(e, struct hash_item, elem)->data;
		return (unsigned) hash_int(data);

}
bool callPinFun( char toks[TOKMAX][TOKLENMAX] )
{

		struct list_elem * e = NULL;
		int val = -1;
		bool call = false;
		int i;
		if(!strcmp(toks[0],"list_push_front"))
		{
				for(i = 0;i<=namedListCnt;i++)
				{
						
						if(!strcmp(namedListArr[i].name,toks[1]))
						{
								struct list_item *thisItem = malloc(sizeof(struct list_item));
								thisItem->data = atoi(toks[2]); 

								list_push_front(namedListArr[i].thisList,&(thisItem->elem));

								call = true;
						}
			}
		}
		else if(!strcmp(toks[0],"list_push_back"))
		{
				int listIdx = getNamedListIdx(toks[1]);
				if(listIdx!=-1)
				{
						struct list_item *thisItem = malloc(sizeof(struct list_item));
						thisItem->data = atoi(toks[2]); 

						list_push_back(namedListArr[listIdx].thisList,&(thisItem->elem));

						call = true;
					
				}

		}
		else if(!strcmp(toks[0],"list_insert"))
		{
				int listIdx = getNamedListIdx(toks[1]);
				if(listIdx!=-1)
				{
						int insertPos = atoi(toks[2]), i;
						struct list_item *thisItem = malloc(sizeof(struct list_item));
						struct list_elem *e;
						thisItem->data = atoi(toks[3]); 
						i = 0;
						for(e = list_begin(namedListArr[listIdx].thisList);e!=list_end(namedListArr[listIdx].thisList);e = list_next(e))
						{
								if(i<insertPos)
									i++;
								else
										break;
						}
						if(insertPos == i)
						{	
							list_insert(e,&(thisItem->elem));
							call = true;
						}
				}
		}

		else if(!strcmp(toks[0],"list_splice"))
		{
				call = call_list_splice(toks);
		}
		else if(!strcmp(toks[0],"list_remove"))
		{
				call = call_list_remove(toks);
		}
		else if(!strcmp(toks[0],"list_pop_front"))
		{
				e = call_list_pop_front(toks);
				if(e) call = true;
		}
		else if(!strcmp(toks[0],"list_pop_back"))
		{
				e = call_list_pop_back(toks);
				if(e) call = true;
		}
		else if(!strcmp(toks[0],"list_front"))
		{
				//print = true;
				e = call_list_front(toks);
				if(e) call = true;
		}
		else if(!strcmp(toks[0],"list_back"))
		{
				//print = true;
				e = call_list_back(toks);
				if(e) call = true;
		}
		else if(!strcmp(toks[0],"list_size"))
		{
				val = call_list_size(toks);
				if(val!=-1)
						call = true;
		}
		else if(!strcmp(toks[0],"list_empty"))
		{
				call_list_empty(toks);
				call = true;////    아몰랑
		}
		else if(!strcmp(toks[0],"list_max"))
		{
				val = call_list_max(toks);
				if(val!= -10000)////  아몰랑
						call = true;
		}
		else if(!strcmp(toks[0], "list_min"))
		{
				val = call_list_min(toks);
				if(val!=10000)//// 아몰랑
				{
						call = true;
				}
		}
		else if(!strcmp(toks[0], "list_reverse"))
		{
				call = call_list_reverse(toks);

		}
		else if(!strcmp(toks[0],"list_sort"))
		{
				call = call_list_sort(toks);
		}
		else if(!strcmp(toks[0],"list_insert_ordered"))
		{
				call = call_list_insert_ordered(toks);
		}
		else if(!strcmp(toks[0],"list_unique"))
		{
				call = call_list_unique(toks);
		}
		else if(!strcmp(toks[0],"list_swap"))
		{
				call = true;
				call_list_swap(toks);
		}
		else if(!strcmp(toks[0],"list_shuffle"))
		{
				call = true;
				call_list_shuffle(toks);
		}


	/////i///////////////////////////hash////////////////////////////////////
		///////////////////////////////////////////////////////////////////


		else if(!strcmp(toks[0],"hash_insert"))
		{
				call = true;
				call_hash_insert(toks);
		}
		else if(!strcmp(toks[0],"hash_replace"))
		{
				call = true;
				call_hash_replace(toks);
		}
		else if(!strcmp(toks[0],"hash_find"))
		{
				call = true;
				call_hash_find(toks);
		}
		else if(!strcmp(toks[0], "hash_delete"))
		{	
				call = true;
				call_hash_delete(toks);
		}
		else if(!strcmp(toks[0], "hash_clear"))
		{	
				call = true;
				call_hash_clear(toks);
		}
		else if(!strcmp(toks[0], "hash_size"))
		{	
				call = true;
				call_hash_size(toks);
		}
		else if(!strcmp(toks[0], "hash_empty"))
		{	
				call = true;
				call_hash_empty(toks);
		}
		else if(!strcmp(toks[0], "hash_apply"))
		{	
				call = true;
				call_hash_apply(toks);
		}







		////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////


		///////////////////////////bitmap/////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////
		
		else if(!strcmp(toks[0], "bitmap_expand"))
		{
				call =true;
				call_bitmap_expand(toks);
		}
		else if(!strcmp(toks[0], "bitmap_mark"))
		{	
				call = true;
				call_bitmap_mark(toks);
		}
		else if(!strcmp(toks[0], "bitmap_size"))
		{	
				call = true;
				call_bitmap_size(toks);
		}
		else if(!strcmp(toks[0], "bitmap_set"))
		{	
				call = true;
				call_bitmap_set(toks);
		}
		else if(!strcmp(toks[0], "bitmap_reset"))
		{	
				call = true;
				call_bitmap_reset(toks);
		}
		else if(!strcmp(toks[0], "bitmap_flip"))
		{	
				call = true;
				call_bitmap_flip(toks);
		}
		else if(!strcmp(toks[0], "bitmap_test"))
		{	
				call = true;
				call_bitmap_test(toks);
		}
		else if(!strcmp(toks[0], "bitmap_set_all"))
		{	
				call = true;
				call_bitmap_set_all(toks);
		}
		else if(!strcmp(toks[0], "bitmap_set_multiple"))
		{	
				call = true;
				call_bitmap_set_multiple(toks);
		}
		else if(!strcmp(toks[0], "bitmap_count"))
		{	
				call = true;
				call_bitmap_count(toks);
		}
		else if(!strcmp(toks[0], "bitmap_contains"))
		{	
				call = true;
				call_bitmap_contains(toks);
		}
		else if(!strcmp(toks[0], "bitmap_any"))
		{	
				call = true;
				call_bitmap_any(toks);
		}
		else if(!strcmp(toks[0], "bitmap_none"))
		{	
				call = true;
				call_bitmap_none(toks);
		}
		else if(!strcmp(toks[0], "bitmap_all"))
		{	
				call = true;
				call_bitmap_all(toks);
		}
		else if(!strcmp(toks[0], "bitmap_scan"))
		{	
				call = true;
				call_bitmap_scan(toks);
		}
		else if(!strcmp(toks[0], "bitmap_scan_and_flip"))
		{	
				call = true;
				call_bitmap_scan_and_flip(toks);
		}
		else if(!strcmp(toks[0], "bitmap_dump"))
		{	
				call = true;
				call_bitmap_dump(toks);
		}









		//////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////
		
		//////////////////print///////////////////
	/*	if(e)//pops listfront listback
		{
				call = true;
				if(print)//listfrnt listback
				{
					item = list_entry(e,struct list_item,elem);
					printf("%d\n", item->data);
				}
		}
*/
/*		if(val!= -1) // size, max, min
		{
				call = true;
				printf("%d\n", val);
		}*/
/*		if(condition)//empty
		{
				if(condition)
						printf("true\n");
				else
						printf("false\n");
		}*/
		///////////////////////////////////////////
		return call;
}
void call_hash_insert(char toks[TOKMAX][TOKLENMAX])
{
		int hashIdx = getNamedHashIdx(toks[1]);
		if(hashIdx!=-1)
		{
				struct hash_item *thisItem = malloc(sizeof(struct hash_item));
				thisItem->data = atoi(toks[2]); 
				hash_insert(namedHashArr[hashIdx].thisHash,&(thisItem->elem));
		}
	
}

void call_hash_replace(char toks[TOKMAX][TOKLENMAX])
{
		int hashIdx = getNamedHashIdx(toks[1]);
		if(hashIdx!=-1)
		{
				struct hash_item *thisItem = malloc(sizeof(struct hash_item));
				thisItem->data = atoi(toks[2]); 
			
				hash_replace(namedHashArr[hashIdx].thisHash,&(thisItem->elem));
		}
	
}

void call_hash_find(char toks[TOKMAX][TOKLENMAX])
{
		int hashIdx = getNamedHashIdx(toks[1]);
		if(hashIdx!=-1)
		{
				struct hash_item *thisItem = malloc(sizeof(struct hash_item));
				struct hash_elem * e;
				thisItem->data = atoi(toks[2]); 
			
				e = hash_find(namedHashArr[hashIdx].thisHash,&(thisItem->elem));
				if(e)printf("%d\n",hash_entry(e, struct hash_item,elem)->data);
		}
	
}

void call_hash_delete(char toks[TOKMAX][TOKLENMAX])
{
		int hashIdx = getNamedHashIdx(toks[1]);
		if(hashIdx!=-1)
		{
				struct hash_item *thisItem = malloc(sizeof(struct hash_item));
				thisItem->data = atoi(toks[2]); 
			
				hash_delete(namedHashArr[hashIdx].thisHash,&(thisItem->elem));
//				if(e)printf("%d\n",hash_entry(e, struct hash_item,elem)->data);
		}
	
}

void call_hash_clear(char toks[TOKMAX][TOKLENMAX])
{
		int hashIdx = getNamedHashIdx(toks[1]);
		if(hashIdx!=-1)
		{
				hash_clear(namedHashArr[hashIdx].thisHash,hash_destructor);
//				if(e)printf("%d\n",hash_entry(e, struct hash_item,elem)->data);
		}
	
}

void hash_destructor(struct hash_elem *a, void *aux)
{
		free(hash_entry(a, struct hash_item, elem));
}

void call_hash_size(char toks[TOKMAX][TOKLENMAX])
{
		int hashIdx = getNamedHashIdx(toks[1]);
		if(hashIdx!=-1)
		{
				size_t size = hash_size(namedHashArr[hashIdx].thisHash);
				printf("%d\n",size);
		}
	

}

void call_hash_empty(char toks[TOKMAX][TOKLENMAX])
{		
		int hashIdx = getNamedHashIdx(toks[1]);
		if(hashIdx!=-1)
		{
				if(hash_empty(namedHashArr[hashIdx].thisHash))
					printf("true\n");
				else
						printf("false\n");
		}
	


}

void call_hash_apply(char toks[TOKMAX][TOKLENMAX])
{
		int hashIdx = getNamedHashIdx(toks[1]);
		if(hashIdx!=-1)
		{
				if(!strcmp(toks[2],"square"))
					hash_apply(namedHashArr[hashIdx].thisHash, square);
					
				else if(!strcmp(toks[2],"triple"))
					hash_apply(namedHashArr[hashIdx].thisHash, triple);
		}
	


}
void square(struct hash_elem *e, void *aux)
{
	struct hash_item * item = hash_entry(e,struct hash_item, elem);
	item->data = (item->data) * (item -> data);
}
void triple(struct hash_elem *e, void *aux)
{	
	struct hash_item * item = hash_entry(e,struct hash_item, elem);
	item->data = (item->data) * (item -> data) *(item -> data);
}

void call_bitmap_mark(char toks[TOKMAX][TOKLENMAX])
{
	int bitmapIdx = getNamedBitmapIdx(toks[1]);
		if(bitmapIdx!=-1)
		{
				bitmap_mark(namedBitmapArr[bitmapIdx].thisBitmap, (size_t)atoi(toks[2]));
		}
	

}


void call_bitmap_size(char toks[TOKMAX][TOKLENMAX])
{
	int bitmapIdx = getNamedBitmapIdx(toks[1]);
		if(bitmapIdx!=-1)
		{
			printf("%d\n",	(int)bitmap_size(namedBitmapArr[bitmapIdx].thisBitmap));
		}
	
}

void call_bitmap_set(char toks[TOKMAX][TOKLENMAX])
{
	int bitmapIdx = getNamedBitmapIdx(toks[1]);
	if(bitmapIdx!=-1)
	{
	//	bool set;
	//	set = !strcmp(toks[3],"true");
		bitmap_set(namedBitmapArr[bitmapIdx].thisBitmap, (size_t)atoi(toks[2]),!strcmp(toks[3],"true"));
	}
	

	
}
void call_bitmap_reset(char toks[TOKMAX][TOKLENMAX])
{
	int bitmapIdx = getNamedBitmapIdx(toks[1]);
	if(bitmapIdx!=-1)
	{
	//	bool set;
	//	set = !strcmp(toks[3],"true");
		bitmap_reset(namedBitmapArr[bitmapIdx].thisBitmap, (size_t)atoi(toks[2]));
	}
	
}
void call_bitmap_flip(char toks[TOKMAX][TOKLENMAX])
{
	int bitmapIdx = getNamedBitmapIdx(toks[1]);
	if(bitmapIdx!=-1)
	{
	//	bool set;
	//	set = !strcmp(toks[3],"true");
		bitmap_flip(namedBitmapArr[bitmapIdx].thisBitmap, (size_t)atoi(toks[2]));
	}
	
}
void call_bitmap_test(char toks[TOKMAX][TOKLENMAX])
{
	int bitmapIdx = getNamedBitmapIdx(toks[1]);
	if(bitmapIdx!=-1)
	{
	//	bool set;
	//	set = !strcmp(toks[3],"true");
		if(bitmap_test(namedBitmapArr[bitmapIdx].thisBitmap, (size_t)atoi(toks[2])))
				printf("true\n");
		else
				printf("false\n");
	}
	
}
void call_bitmap_set_all(char toks[TOKMAX][TOKLENMAX])
{
	int bitmapIdx = getNamedBitmapIdx(toks[1]);
	if(bitmapIdx!=-1)
	{
	//	bool set;
	//	set = !strcmp(toks[3],"true");
		bitmap_set_all(namedBitmapArr[bitmapIdx].thisBitmap, !strcmp(toks[2],"true"));
	}
	
}
void call_bitmap_set_multiple(char toks[TOKMAX][TOKLENMAX])
{
	int bitmapIdx = getNamedBitmapIdx(toks[1]);
	if(bitmapIdx!=-1)
	{
	//	bool set;
	//	set = !strcmp(toks[3],"true");
		bitmap_set_multiple(namedBitmapArr[bitmapIdx].thisBitmap,(size_t)atoi(toks[2]),(size_t)atoi(toks[3]), !strcmp(toks[4],"true"));
	}
	
}

void call_bitmap_expand(char toks[TOKMAX][TOKLENMAX])
{
	int bitmapIdx = getNamedBitmapIdx(toks[1]);
	if(bitmapIdx!=-1)
	{
	//	bool set;
	//	set = !strcmp(toks[3],"true");
			bitmap_expand(namedBitmapArr[bitmapIdx].thisBitmap,atoi(toks[2]));
	}
	
}
void call_bitmap_count(char toks[TOKMAX][TOKLENMAX])
{
	int bitmapIdx = getNamedBitmapIdx(toks[1]);
	if(bitmapIdx!=-1)
	{
	//	bool set;
	//	set = !strcmp(toks[3],"true");
		printf("%d\n",(int)bitmap_count(namedBitmapArr[bitmapIdx].thisBitmap,(size_t)atoi(toks[2]),(size_t)atoi(toks[3]), !strcmp(toks[4],"true")));
	}
	
}
void call_bitmap_contains(char toks[TOKMAX][TOKLENMAX])
{	
	int bitmapIdx = getNamedBitmapIdx(toks[1]);
	if(bitmapIdx!=-1)
	{
	//	bool set;
	//	set = !strcmp(toks[3],"true");
		if(bitmap_contains(namedBitmapArr[bitmapIdx].thisBitmap,(size_t)atoi(toks[2]),(size_t)atoi(toks[3]), !strcmp(toks[4],"true")))
				printf("true\n");
		else
				printf("false\n");
	}
	
}
void call_bitmap_any(char toks[TOKMAX][TOKLENMAX])
{
	int bitmapIdx = getNamedBitmapIdx(toks[1]);
	if(bitmapIdx!=-1)
	{
	//	bool set;
	//	set = !strcmp(toks[3],"true");
		if(bitmap_any(namedBitmapArr[bitmapIdx].thisBitmap,(size_t)atoi(toks[2]),(size_t)atoi(toks[3])))
				printf("true\n");
		else
				printf("false\n");
	}

}
void call_bitmap_none(char toks[TOKMAX][TOKLENMAX])
{	int bitmapIdx = getNamedBitmapIdx(toks[1]);
	if(bitmapIdx!=-1)
	{
	//	bool set;
	//	set = !strcmp(toks[3],"true");
		if(bitmap_none(namedBitmapArr[bitmapIdx].thisBitmap,(size_t)atoi(toks[2]),(size_t)atoi(toks[3])))
				printf("true\n");
		else
				printf("false\n");
	}

}
void call_bitmap_all(char toks[TOKMAX][TOKLENMAX])
{	int bitmapIdx = getNamedBitmapIdx(toks[1]);
	if(bitmapIdx!=-1)
	{
	//	bool set;
	//	set = !strcmp(toks[3],"true");
		if(bitmap_all(namedBitmapArr[bitmapIdx].thisBitmap,(size_t)atoi(toks[2]),(size_t)atoi(toks[3])))
				printf("true\n");
		else
				printf("false\n");
	}

}
void call_bitmap_scan(char toks[TOKMAX][TOKLENMAX])
{
	int bitmapIdx = getNamedBitmapIdx(toks[1]);
	if(bitmapIdx!=-1)
	{
	//	bool set;
	//	set = !strcmp(toks[3],"true");
		printf("%u\n",bitmap_scan(namedBitmapArr[bitmapIdx].thisBitmap,(size_t)atoi(toks[2]),(size_t)atoi(toks[3]), !strcmp(toks[4],"true")));
	}
		
}
void call_bitmap_scan_and_flip(char toks[TOKMAX][TOKLENMAX])
{	int bitmapIdx = getNamedBitmapIdx(toks[1]);
	if(bitmapIdx!=-1)
	{
	//	bool set;
	//	set = !strcmp(toks[3],"true");
		printf("%u\n",bitmap_scan_and_flip(namedBitmapArr[bitmapIdx].thisBitmap,(size_t)atoi(toks[2]),(size_t)atoi(toks[3]), !strcmp(toks[4],"true")));
	}
	
}
void call_bitmap_dump(char toks[TOKMAX][TOKLENMAX])
{	int bitmapIdx = getNamedBitmapIdx(toks[1]);
	if(bitmapIdx!=-1)
	{
	//	bool set;
	//	set = !strcmp(toks[3],"true");
		bitmap_dump((const struct bitmap *)namedBitmapArr[bitmapIdx].thisBitmap);
	}
	
}

bool call_list_unique(char toks[TOKMAX][TOKLENMAX])
{			
		int listIdx1 = getNamedListIdx(toks[1]), listIdx2 = getNamedListIdx(toks[2]);
		bool call = false;


		if(listIdx1!=-1)
		{
				if(listIdx2==-1)
				{

						list_unique(namedListArr[listIdx1].thisList,NULL,list_itemData_less, NULL);
				}
				else
						list_unique(namedListArr[listIdx1].thisList,namedListArr[listIdx2].thisList,list_itemData_less, NULL);
				call = true;
		}
	
		return call;
}

bool call_list_insert_ordered(char toks[TOKMAX][TOKLENMAX])
{			
		int listIdx = getNamedListIdx(toks[1]);
		bool call = false;
		if(listIdx!=-1)
		{
				struct list_item *thisItem = malloc(sizeof(struct list_item));
				thisItem->data = atoi(toks[2]); 
				list_insert_ordered(namedListArr[listIdx].thisList,&(thisItem->elem),list_itemData_less, NULL);
				call = true;
		}
	
		return call;
}
bool call_list_sort(char toks[TOKMAX][TOKLENMAX])
{	
		bool call = false;
		int listIdx = getNamedListIdx(toks[1]);
		if(listIdx!=-1)
		{
				list_sort(namedListArr[listIdx].thisList, list_itemData_less, NULL);
				call = true;
		}

		return call;
}

bool call_list_reverse(char toks[TOKMAX][TOKLENMAX])
{	
		bool call = false;
		int listIdx = getNamedListIdx(toks[1]);
		if(listIdx!=-1)
		{
				list_reverse(namedListArr[listIdx].thisList);
				call = true;
		}

		return call;
}

void call_list_swap(char toks[TOKMAX][TOKLENMAX])
{		
		struct list_elem *a, *b;
		
		int listIdx = getNamedListIdx(toks[1]);
		if(listIdx!= -1)
		{
				a = getElem_idx(listIdx, atoi(toks[2]));
				b = getElem_idx(listIdx, atoi(toks[3]));
				
		//		printf("a b :: %d %d\n",list_entry(a, struct list_item, elem)->data, list_entry(b, struct list_item, elem)->data);
				list_swap(a,b);
				
		}
}

void call_list_shuffle(char toks[TOKMAX][TOKLENMAX])
{
		int listIdx = getNamedListIdx(toks[1]);
		if(listIdx!= -1)
		{
				
		//		printf("a b :: %d %d\n",list_entry(a, struct list_item, elem)->data, list_entry(b, struct list_item, elem)->data);
				list_shuffle(namedListArr[listIdx].thisList);
				
		}

}
int call_list_min(char toks[TOKMAX][TOKLENMAX])
{
		int min = 10000;
		struct list_elem * e = NULL;
		struct list_item * item;
		int listIdx = getNamedListIdx(toks[1]);
		if(listIdx!= -1)
		{
				e = list_min(namedListArr[listIdx].thisList, list_itemData_less, NULL);
				if(e)
				{
					item = list_entry(e,struct list_item,elem);
					min = item->data;
					printf("%d\n", min);
				}
		}

		return min;


}
int call_list_max(char toks[TOKMAX][TOKLENMAX])
{
		int max = -10000;////  아몰랑
		struct list_elem * e = NULL;
		struct list_item * item;
		int listIdx = getNamedListIdx(toks[1]);
		if(listIdx!= -1)
		{
				e = list_max(namedListArr[listIdx].thisList, list_itemData_less, NULL);
				if(e)
				{
					item = list_entry(e,struct list_item,elem);
					max = item->data;
					printf("%d\n", max);
				}
		}

		return max;

}
bool list_itemData_less(const struct list_elem * elem1, const struct list_elem * elem2, void* aux)
{

		// aux 에 뭐씀?????
//		list_item *item1, *item2;
		int data1, data2;
		data1 = list_entry(elem1,struct list_item,elem)->data;
		data2 = list_entry(elem2,struct list_item,elem)->data;

		if(data1<data2)
				return true;
		else
				return false;
}
bool call_list_empty(char toks[TOKMAX][TOKLENMAX])
{
		bool isEmpty = false;
		int listIdx = getNamedListIdx(toks[1]);
		if(listIdx!=-1)
		{
			isEmpty = list_empty(namedListArr[listIdx].thisList);
			if(isEmpty)
					printf("true\n");
			else
					printf("false\n");
		}
		return isEmpty;
}
int call_list_size(char toks[TOKMAX][TOKLENMAX])
{
		int size = -1;
		int listIdx = getNamedListIdx(toks[1]);
		if(listIdx!=-1)
		{
			size = list_size(namedListArr[listIdx].thisList);
			printf("%d\n", size);
		}
		return size;
}
struct list_elem * call_list_back(char toks[TOKMAX][TOKLENMAX])
{
		struct list_item *item;
		struct list_elem * e = NULL;
		int listIdx = getNamedListIdx(toks[1]);
		if(listIdx!=-1)
		{
				e = list_back(namedListArr[listIdx].thisList);
				item = list_entry(e,struct list_item,elem);
				printf("%d\n", item->data);
		}

		return e;

}

struct list_elem * call_list_front(char toks[TOKMAX][TOKLENMAX])
{
		struct list_item *item;
		struct list_elem * e = NULL;
		int listIdx = getNamedListIdx(toks[1]);
		if(listIdx!=-1)
		{
				e = list_front(namedListArr[listIdx].thisList);
				item = list_entry(e,struct list_item,elem);
				printf("%d\n", item->data);
		}

		return e;

}
struct list_elem * call_list_pop_front(char toks[TOKMAX][TOKLENMAX])
{
		struct list_elem * e = NULL;
		int listIdx = getNamedListIdx(toks[1]);
		if(listIdx!=-1)
		{
				e = list_pop_front(namedListArr[listIdx].thisList);
		}

		return e;
}
struct list_elem * call_list_pop_back(char toks[TOKMAX][TOKLENMAX])
{
		struct list_elem * e = NULL;
		int listIdx = getNamedListIdx(toks[1]);
		if(listIdx!=-1)
		{
				e = list_pop_back(namedListArr[listIdx].thisList);
		}

		return e;
}

bool call_list_remove(char toks[TOKMAX][TOKLENMAX] )
{
		bool call = false;
		int listIdx = getNamedListIdx(toks[1]);
		int pos = atoi(toks[2]);
		struct list_elem *e = NULL;
		if(listIdx!=-1)
			e = getElem_idx(listIdx, pos);
		if(e)
		{
			list_remove(e);
			call=true;
		}
		return call;
}
bool call_list_splice(char toks[TOKMAX][TOKLENMAX] )
{
		struct list_elem *before, *first, *last;
		bool call = false;
		int listIdx_dest = getNamedListIdx(toks[1]), listIdx_src = getNamedListIdx(toks[3]);
		int destPos = atoi(toks[2]), srcPosFrom = atoi(toks[4]), srcPosTo = atoi(toks[5]);
		if(listIdx_dest != -1 && listIdx_src != -1)
		{
			before = getElem_idx(listIdx_dest,destPos);
			first = getElem_idx(listIdx_src, srcPosFrom);
			last = getElem_idx(listIdx_src, srcPosTo);
			if(before&&first&&last)
			{
		
				list_splice(before, first, last);
				call=true;
			}
		}
		return call;
}
struct list_elem*  getElem_idx(int listIdx, int pos)
{						
	int i = 0;
	struct list_elem *e;
		
	for(e = list_begin(namedListArr[listIdx].thisList);e!=list_end(namedListArr[listIdx].thisList);e = list_next(e))
	{
		if(i<pos)
			i++;
		else if(i==pos)
				return e;
		else break;
	}
		return NULL;
}
int getNamedListIdx(char* name)
{
	int i;
	for(i = 0;i<=namedListCnt;i++)
	{
						
				if(!strcmp(namedListArr[i].name,name))
				{
						return i;
				}
	}
	return -1;
}
int getNamedHashIdx(char* name)
{
	int i;
	for(i = 0;i<=namedHashCnt;i++)
	{
						
				if(!strcmp(namedHashArr[i].name,name))
				{
						return i;
				}
	}
	return -1;
}
int getNamedBitmapIdx(char* name)
{
	int i;
	for(i = 0;i<=namedBitmapCnt;i++)
	{
						
				if(!strcmp(namedBitmapArr[i].name,name))
				{
						return i;
				}
	}
	return -1;
}


int getCmdIdx(char* cmdBuff)
{
		int cmdIdx = -1;
		if(strcmp(cmdBuff,"quit"))
				cmdIdx = 0;
		
		return cmdIdx;
}

void makeToken(char buff[BUFFMAX], char toks[TOKMAX][TOKLENMAX] )
{
		int tokI = 0;
		char* aTok;
		aTok = strtok(buff," \n");
		while(aTok!=NULL)
		{
			strncpy(toks[tokI++],aTok,TOKLENMAX);
			aTok = strtok(NULL, " \n");
		}
		for(;tokI<TOKMAX;tokI++)
		{
				strcpy(toks[tokI],"\0");
		}
}
bool callCre( char toks[TOKMAX][TOKLENMAX] )
{
		bool create = false;

		if(!strcmp(toks[1],"list"))
				create = callCreL(toks);
		else if(!strcmp(toks[1],"hashtable"))
		{
				create = true;
				callCreH(toks);
		}
		else if(!strcmp(toks[1],"bitmap"))
		{
				create = true;
				callCreB(toks);
		}
		return create;
}
void callCreB(char toks[TOKMAX][TOKLENMAX])
{
		if(namedBitmapCnt < BITMAPMAX-1)
		{
				strncpy(namedBitmapArr[++namedBitmapCnt].name, toks[2],TOKLENMAX);
				namedBitmapArr[namedBitmapCnt].thisBitmap = bitmap_create((size_t)atoi(toks[3]));
		//	namedListCnt++;

		}
}

void callCreH(char toks[TOKMAX][TOKLENMAX])
{
		if(namedHashCnt < HASHMAX-1)
		{
				strncpy(namedHashArr[++namedHashCnt].name, toks[2],TOKLENMAX);
				namedHashArr[namedHashCnt].thisHash = malloc(sizeof(struct hash));
				hash_init(namedHashArr[namedHashCnt].thisHash, hash_func, hash_data_less,NULL);
		//	namedListCnt++;

		}
}
bool hash_data_less(const struct hash_elem *a, const struct hash_elem *b, void *aux)
{
		int data1, data2;
		
		data1 = hash_entry(a,struct hash_item, elem)->data;
		data2 = hash_entry(b,struct hash_item, elem)->data;

		if(data1<data2) return true;
		else return false;

}
bool callCreL( char toks[TOKMAX][TOKLENMAX] )
{
	bool create = false;
	if(namedListCnt<LISTMAX-1)
	{
			strncpy(namedListArr[++namedListCnt].name, toks[2],TOKLENMAX);
			namedListArr[namedListCnt].thisList = malloc(sizeof(struct list));
			namedListArr[namedListCnt].thisList->head.prev = NULL;
			namedListArr[namedListCnt].thisList->head.next = &(namedListArr[namedListCnt].thisList->tail);
			namedListArr[namedListCnt].thisList->tail.prev = &(namedListArr[namedListCnt].thisList->head);
			namedListArr[namedListCnt].thisList->tail.next = NULL;
		//	namedListCnt++;
			create = true;
	}
	return create;
}
void callDel( char toks[TOKMAX][TOKLENMAX] )
{
	
	int listIdx = getNamedListIdx(toks[1]);
	int hashIdx = getNamedHashIdx(toks[1]);
	int bitmapIdx = getNamedBitmapIdx(toks[1]);
	struct list_elem *e;
	;
	if(listIdx!= -1) 
	{
			
			for(e = list_begin(namedListArr[listIdx].thisList);e!=list_end(namedListArr[listIdx].thisList);e = list_next(e))
			{
					list_remove(e);
			}
			free(namedListArr[listIdx].thisList);
			strcpy(namedListArr[listIdx].name,"\0");
	}

	else if(hashIdx!= -1)
	{
			struct hash * h = namedHashArr[hashIdx].thisHash;
	
			hash_clear(h,hash_destructor);
			free(namedHashArr[hashIdx].thisHash);
			strcpy(namedHashArr[hashIdx].name,"\0");
	

	}
	else if(bitmapIdx!= -1)
	{
			
			struct bitmap * thisBitmap = namedBitmapArr[bitmapIdx].thisBitmap;
			bitmap_destroy(thisBitmap);
			free(namedBitmapArr[listIdx].thisBitmap);
			strcpy(namedBitmapArr[listIdx].name,"\0");
	

	}


}
bool callDum( char toks[TOKMAX][TOKLENMAX] )
{
	bool call = false;
	int listIdx = getNamedListIdx(toks[1]);
	int hashIdx = getNamedHashIdx(toks[1]);
	int bitmapIdx = getNamedBitmapIdx(toks[1]);
	struct list_elem *e;
	if(listIdx!= -1) 
	{
			call = true;
			for(e = list_begin(namedListArr[listIdx].thisList);e!=list_end(namedListArr[listIdx].thisList);e = list_next(e))
			{
					struct list_item *item = list_entry(e,struct list_item,elem);
					printf("%d ", item->data);
			}
			printf("\n");
	}

	else if(hashIdx!= -1)
	{
			struct hash * h = namedHashArr[hashIdx].thisHash;
	  		struct hash_iterator i;
			int cnt = 0;
   		   hash_first (&i, h);
     		 while (hash_next (&i))
       		 {	
         		 struct hash_item *f = hash_entry (hash_cur (&i), struct hash_item, elem);
       		   		
				 printf("%d ",f->data);
				 cnt++;
       		 }
			if(cnt)printf("\n");


	}
	else if(bitmapIdx!= -1)
	{
			int i, cnt = 0;
			struct bitmap * thisBitmap = namedBitmapArr[bitmapIdx].thisBitmap;
			for(i = 0;i<bitmap_size(thisBitmap);i++)
			{
					cnt++;
					printf("%d",bitmap_test(thisBitmap,i));
			}

			if(cnt) printf("\n");
	}

	return call;
}
void callCom( char toks[TOKMAX][TOKLENMAX] )
{
}
