
void _buddy_return_page(page_t* page) {
    // TODO: make merge here
    // Suggested: 4 LoCs
}

void _buddy_get_specific_page(page_t* page) {
    page->flags |= BD_PAGE_IN_USE;
    bd_lists[page->orders].nr_free--;
    list_del(&page->list_node);
}

void _buddy_clear_flag(page_t* page) { page->flags = 0; }

uint64 _buddy_get_page_idx(page_t* page) { return /* Your return value */NULL; }

uint64 _buddy_get_area_idx(void* head) { return /* Your return value */NULL; }

page_t* _buddy_idx_get_page(uint64 idx) {
    return /* Your return value */NULL;
}

page_t* _buddy_get_buddy_page(page_t* page) {
    // Suggested: 2 LoCs
    return /* Your return value */NULL;
}

void _buddy_split_page(page_t *original_page, uint64 target_order){
    // Suggested: 5 LoCs
}

page_t *_buddy_alloc_page(uint64 order){
    // Suggested: 13 LoCs
    return /* Your return value */NULL;
}

void buddy_free_page(page_t* page) {
    for (; page->orders < bd_max_size; ++page->orders) {
        page_t* buddy_page = _buddy_get_buddy_page(page);
        if ((!(buddy_page->flags & BD_PAGE_FREE))) {
            break;
        }
        _buddy_get_specific_page(buddy_page);
        _buddy_clear_flag(buddy_page);
    }
    _buddy_return_page(page);
}