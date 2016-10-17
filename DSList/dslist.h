#ifndef D_S_LIST
#define D_S_LIST


typedef void *Pointer;

typedef struct tDSList {
    Pointer data;
    struct tDSList *next;
    struct tDSList *prev;
} DSList;

/* Добавить в хвост. Возвращает новое начало списка. */
DSList *dslist_append(DSList *list, Pointer data);

/* Добавить в голову. Возвращает новое начало списка. */
DSList *dslist_prepend(DSList *list, Pointer data);

/* Вставить после указанного элемента */
int dslist_insert(DSList *sibling, Pointer data);

/* Удалить элемент из списка и освободить память.
   Возвращает новое начало списка. */
DSList *dslist_remove(DSList*list, Pointer data);

/* Удалить все элементы из списка со значением data и освободить память.
   Возвращает новое начало списка. */
DSList*dslist_remove_all(DSList*list, Pointer data);

/* Удалить элемент, следующий за sibling.
   Возвращает данные из удаленного элемента */
Pointer dslist_remove_next(DSList*sibling);

/* Уничтожить весь список */
void dslist_free(DSList*list);

/* Подсчитать длину списка */
size_t dslist_length(DSList*list);

/* Скопировать список. Возвращает начало копии */
DSList*dslist_copy(DSList*list);

/* Перевернуть список. Возвращает новое начало списка (бывший хвост) */
DSList*dslist_reverse(DSList*list);

/* Конкатенация двух списков (присоединяет list2 к list1).
   Возвращает указатель на начало объединенного списка */
DSList*dslist_concat(DSList*list1, DSList*list2);

/* Обход списка. Функции func на каждом шаге передаются два аргумента:
   data, соответствующий данным текущего элемента списка, и user_data,
   являющийся аргументом самой функции */
void dslist_foreach(DSList*list,
                    void (*func)(Pointer data, Pointer user_data), Pointer user_data);

/* Последний элемент списка */
DSList*dslist_last(DSList*list);

DSList*dslist_first(DSList*list);

/* n-й элемент списка (или 0). Если n отрицательно, элементы
   считаются с конца (-1 -- последний и т.д.) */
DSList*dslist_nth(DSList*list, int n);

/* Поиск элемента со значением данных data (0 если не найден) */
DSList*dslist_find(DSList*haystack, Pointer needle);

/* Возвращает первый элемент, для которого compare_func вернет 0,
   сравнивая каждый элемент списка с needle */
DSList*dslist_find_custom(DSList*haystack, Pointer needle,
                          int (*compare_func)(Pointer a, Pointer b));

/* Возвращает позицию элемента el в списке или -1,
   если он не присутствует в списке */
int dslist_position(DSList*list, DSList*el);


#endif
