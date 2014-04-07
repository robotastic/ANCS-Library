/** (c)2013, Bernard Pratz, bernard at pratz dot net
 *  under the WTFPL License
 */

#include "ancs_base.h"
#include "ancs_notification.h"
#include "ancs_notification_list.h"

#include "data_lib/utilities.h"

#define CACHE_SIZE 10

// keep a buffer for each kind

static ancs_notification_t* _notification_buffer[CACHE_SIZE];
static uint8_t _first_index = 0;
static uint8_t _last_index = 0;

void* _ancs_notification_list_alloc() {
    // cout << F("[ANCS NL] _ancs_notification_list_alloc()") << endl;
    free_ram();
    return NULL;
}
void ancs_notification_list_init () {
    // cout << F("[ANCS NL] ancs_notification_list_init()") << endl;
    free_ram();
    // noop
    return;
}
void ancs_notification_list_push(ancs_notification_t* notif) {
    // cout << F("[ANCS NL] ancs_notification_list_push()") << endl;
    free_ram();

    ++_last_index;

    // if last index has reached CACHE_SIZE, make it cycle
    if (_last_index == CACHE_SIZE)
        _last_index = 0;
    // if last index has reached first index, step first index
    if (_last_index == _first_index)
        ++_first_index;

    _notification_buffer[_last_index] = (ancs_notification_t*)malloc(sizeof(ancs_notification_t));

    // copy notification in place the last index
    ancs_notification_copy(_notification_buffer[_last_index], notif);

    return;
}
ancs_notification_t* ancs_notification_list_get(uint8_t nid) {
    // cout << F("[ANCS NL] ancs_notification_list_get()") << endl;
    free_ram();
    for (uint8_t i=0; i<CACHE_SIZE; ++i)
        if ((_first_index < _last_index && i < _last_index && i > _first_index)
                && _notification_buffer[i]->uid == nid)
            return _notification_buffer[i];
    return NULL;
}
ancs_notification_t* ancs_notification_list_pull() {
    // cout << F("[ANCS NL] ancs_notification_list_pull()") << endl;
    free_ram();
    if (_last_index == _first_index)
        return NULL;
    return _notification_buffer[_last_index];
}
bool ancs_notification_list_remove() {
    // cout << F("[ANCS NL] ancs_notification_list_remove()") << endl;
    free_ram();

    // if buffer is empty, return false
    if (_last_index == _first_index)
        return false;

    // if last index is at first value
    if (0 == _last_index)
        _last_index = CACHE_SIZE-1;
    // otherwise move last index back
    else
        --_last_index;

    return true;
}
ancs_notification_t* ancs_notification_list_pop() {
    // cout << F("[ANCS NL] ancs_notification_list_pop()") << endl;
    free_ram();
    ancs_notification_t* notif = ancs_notification_list_pull();
    if (ancs_notification_list_remove())
        return notif;
    return NULL;
}

void ancs_notification_list_apply(apply_cb cb) {
    // cout << F("[ANCS NL] ancs_notification_list_apply()") << endl;
    free_ram();
    ancs_notification_t* notif = NULL;
    notif = ancs_notification_list_pop();
    cout << "Value of notif: " << (uint32_t)notif << endl;
    if (notif == NULL) {
        cout << "Notif is null" << endl;
    } else {
        cout << "Parsing notif #" << notif->uid << endl;
    }
        cb (notif);
    cout << "other notifs..." << endl;

    while ((notif = ancs_notification_list_pop()) != NULL) {
        cout << "PARSING NOTIF #" << notif->uid << endl;
        cb(notif);
    }
    return;
}
