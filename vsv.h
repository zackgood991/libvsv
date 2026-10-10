/**
 * @file vsv.h
 * @brief Base include for applications.
 * @details The header file for the Very Stupid Vectors library.
 * @author Anonymous
 * @date 2026-10-09
 */


#include <stddef.h>

/**
 * @struct _vsv
 * @brief Opaque "vector" structure used in pointers
 * @note Refrain from accessing this struct
 *       directly, as it is subject to change.
 */
typedef struct _vsv {
	void* ptr;
	size_t count;
	size_t allocd;
	size_t elem;
} VSV;

/**
 * @brief Initialise a vector.
 * @param[in]  elem Size of the elements in the vector.
 * @param[out] self Pointer to VSV struct.
 * @see vsv_free
 */
int vsv_init(size_t elem, VSV* self);
/**
 * @brief Free/destroy a vector.
 * @param[out] self Pointer to VSV struct.
 * @see vsv_init
 */
int vsv_free(VSV* self);

/**
 * @brief Clear the entire vector, and replace it with
 *        n instances of the element x.
 * @param[in]  n Number of elements to insert.
 * @param[in]  x Element to insert.
 * @param[out] self Pointer to VSV struct.
 */
int vsv_assign(size_t n, void* x, VSV* self);
/**
 * @brief Clear the entire vector.
 * @param[out] self Pointer to VSV struct.
 */
int vsv_clear(VSV* self);
/**
 * @brief Insert n instances of element x at index i.
 * @param[in]  i Index before where the content will be inserted.
 * @param[in]  n Number of elements to insert.
 * @param[in]  x Element to insert.
 * @param[out] self Pointer to VSV struct.
 * @see vsv_insert
 * @see vsv_erase
 * @see vsv_erase_n
 */
int vsv_insert_n(size_t i, size_t n, const void* x, VSV* self);
/**
 * @brief Insert an instance of element x at index i.
 * @param[in]  i Index before where the content will be inserted.
 * @param[in]  x Element to insert.
 * @param[out] self Pointer to VSV struct.
 * @see vsv_insert_n
 * @see vsv_erase
 * @see vsv_erase_n
 */
int vsv_insert(size_t i, const void* x, VSV* self);
/**
 * @brief Remove n instances of an element at index i.
 * @param[in]  i Index where the content will be deleted.
 * @param[in]  n Number of elements to remove.
 * @param[out] self Pointer to VSV struct.
 * @see vsv_erase
 * @see vsv_insert
 * @see vsv_insert_n
 */
int vsv_erase_n(size_t i, size_t n, VSV* self);
/**
 * @brief Remove an element at index i.
 * @param[in]  i Index where the content will be deleted.
 * @param[out] self Pointer to VSV struct.
 * @see vsv_erase_n
 * @see vsv_insert
 * @see vsv_insert_n
 */
int vsv_erase(size_t i, VSV* self);
/**
 * @brief Push back an element into the vector.
 * @param[in]  x Pointer to element
 * @param[out] self Pointer to VSV struct.
 * @see pop_back
 */
int vsv_push_back(const void* x, VSV* self);
/**
 * @brief Pops an element out of the back of the vector.
 * @param[out] self Pointer to VSV struct.
 * @see push_back
 */
int vsv_pop_back(VSV* self);
/**
 * @brief Resizes an array, initialising new elements with x.
 * @param[in]  n Size to resize array to.
 * @param[in]  x What to fill new elements with.
 * @param[out] self Pointer to VSV struct.
 * @see vsv_resize
 * @see vsv_assign
 */
int vsv_resize_from(size_t n, void* x, VSV* self);
/**
 * @brief Resizes an array, initialising new elements with 0.
 * @param[in]  n Size to resize array to.
 * @param[out] self Pointer to VSV struct.
 * @see vsv_resize_with
 * @see vsv_assign
 */
int vsv_resize(size_t n, VSV* self);


/**
 * @brief Get a reference to an element from the vector.
 * @param[in]  i Index to element
 * @param[in]  self Pointer to VSV struct.
 * @note Not const due to internal reasons, but
 *       should not modify pointer nevertheless.
 * @see vsv_front
 * @see vsv_back
 */
void* vsv_at(size_t i, VSV* self);
/**
 * @brief Get a reference to the first element from the vector.
 * @param[in]  self Pointer to VSV struct.
 * @note Not const due to internal reasons, but
 *       should not modify pointer nevertheless.
 * @see vsv_at
 * @see vsv_back
 */
void* vsv_front(VSV* self);
/**
 * @brief Get a reference to the last element from the vector.
 * @param[in]  self Pointer to VSV struct.
 * @note Not const due to internal reasons, but
 *       should not modify pointer nevertheless.
 * @see vsv_at
 * @see vsv_front
 */
void* vsv_back(VSV* self);
/**
 * @brief Get a reference to the raw element data from the vector.
 * @param[in]  self Pointer to VSV struct.
 * @note Don't use unless you know what you're doing.
 *       Which you should, because you're writing C.
 */
void* vsv_data(const VSV* self);


/**
 * @brief Get the size from the vector.
 * @param[in] self Pointer to VSV struct.
 */
size_t vsv_size(const VSV* self);
/**
 * @brief Get the capacity from the vector.
 * @param[in] self Pointer to VSV struct.
 */
size_t vsv_capacity(const VSV* self);
/**
 * @brief Get if the vector's size is 0.
 * @param[in] self Pointer to VSV struct.
 * @see vsv_size
 */
int vsv_empty(const VSV* self);
/**
 * @brief Reserve n slots total from the vector.
 * @param[in]  n Number of slots to ensure allocated.
 * @param[out] self Pointer to VSV struct.
 * @note Useful for avoiding unnessacary reallocations.
 * @see vsv_resize
 */
int vsv_reserve(size_t n, VSV* self);
/**
 * @brief Reserve n slots total from the vector.
 * @param[out] self Pointer to VSV struct.
 */
int vsv_shrink_to_fit(VSV* self);
