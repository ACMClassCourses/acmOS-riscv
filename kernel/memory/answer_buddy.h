// 提示：把元数据页加入到空闲池子里
void _buddy_return_page(page_t* page) {
    // TODO: make merge here
    // Suggested: 4 LoCs
}

// 这个函数可以修改
void _buddy_get_specific_page(page_t* page) {
    page->flags |= BD_PAGE_IN_USE;
    bd_lists[page->orders].nr_free--;
    list_del(&page->list_node);
}

void _buddy_clear_flag(page_t* page) { page->flags = 0; }

// 提示：buddy伙伴页的元数据位于整个内存可分配区域的最前面
// 该函数给出一个buddy伙伴页的元数据指针
// 返回元数据所指向的页是全局从数据区开始的第几个页
uint64 _buddy_get_page_idx(page_t* page) { return /* Your return value */NULL; }

// 提示：请仔细阅读kfree函数的代码
uint64 _buddy_get_area_idx(void* head) { return /* Your return value */NULL; }

// 提示：给出一个全局页的下标，返回这个页的元数据指针
page_t* _buddy_idx_get_page(uint64 idx) {
    return /* Your return value */NULL;
}

// 提示：给出一个页的元数据指针，找到这个页的buddy页的元数据指针
// 注意利用buddy页的一些性质
page_t* _buddy_get_buddy_page(page_t* page) {
    // Suggested: 2 LoCs
    return /* Your return value */NULL;
}

// 提示：给出一个buddy页的元数据，将这个页分裂成至少出现1个target_order的函数
void _buddy_split_page(page_t *original_page, uint64 target_order){
    // Suggested: 5 LoCs
}

// 提示：分配一个order为order的页
page_t *_buddy_alloc_page(uint64 order){
    // Suggested: 13 LoCs
    return /* Your return value */NULL;
}

// 这个函数可以修改
void buddy_free_page(page_t* page) {
    page_t* current_page = page;
    for (; current_page->orders < bd_max_size; ++current_page->orders) {
        page_t* buddy_page = _buddy_get_buddy_page(current_page);
        if ((!(buddy_page->flags & BD_PAGE_FREE))) {
            break;
        }
        if(buddy_page->orders != current_page->orders) break;
        page_t* to_be_released = NULL;
        if(_buddy_get_page_idx(current_page) > _buddy_get_page_idx(buddy_page))
            to_be_released = current_page;
        else to_be_released = buddy_page;
        _buddy_get_specific_page(to_be_released);
        _buddy_clear_flag(to_be_released);
        if(_buddy_get_page_idx(current_page) > _buddy_get_page_idx(buddy_page))current_page = buddy_page;
    }
    _buddy_return_page(current_page);
}
