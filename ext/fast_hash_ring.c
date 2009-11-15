#include <fast_hash_ring.h>

void hash_digest(char *key, unsigned char md5pword[16]){
  md5_state_t md5state;

  md5_init(&md5state);
  md5_append(&md5state, (unsigned char *)key, strlen(key));
  md5_finish(&md5state, md5pword);
}

unsigned int hash_val(char *key, int x){
  unsigned char digest[16];

  hash_digest(key, digest);
  return ((digest[3 + x] << 24) |
          (digest[2 + x] << 16) |
          (digest[1 + x] <<  8) |
           digest[0 + x]);
}

static VALUE cFastHashRing_generate_circle(VALUE vself){
  VALUE vnodes, vweights, vring, vsorted_keys;
  int i, j, n, vnodes_len;
  int total_weight = 0;

  vnodes = rb_iv_get(vself, "@nodes");
  vweights = rb_iv_get(vself, "@weights");
  vring = rb_iv_get(vself, "@ring");
  vsorted_keys = rb_iv_get(vself, "@sorted_keys");

  vnodes_len = RARRAY_LEN(vnodes);
  for(i = 0;i < vnodes_len;i++){
    VALUE vnode = rb_ary_entry(vnodes, i);
    VALUE vweight = rb_hash_aref(vweights, vnode);

    if(vweight == Qnil) vweight = INT2NUM(1);
    total_weight += NUM2INT(vweight);
  }

  for(i = 0;i < vnodes_len;i++){
    VALUE vnode = rb_ary_entry(vnodes, i);
    VALUE vweight = rb_hash_aref(vweights, vnode);

    if(vweight == Qnil) vweight = INT2NUM(1);
    int weight = NUM2INT(vweight);

    int factor = floor((40 * vnodes_len * weight) / total_weight);
    for(j = 0;j < factor;j++){
      int blen = 0;
      VALUE vstring = rb_convert_type(vnode, T_STRING, "String", "to_s");
      blen += strlen(RSTRING_PTR(vstring));
      blen += 12;
      char key[blen];
      sprintf(key, "%s-%d", RSTRING_PTR(vstring), j);

      for(n=0;n<3;n++){
        unsigned int hkey = hash_val(key, n*4);
        rb_hash_aset(vring, INT2NUM(hkey), vnode);
        rb_ary_push(vsorted_keys, INT2NUM(hkey));
      }
    }
  }
  rb_iv_set(vself, "@sorted_keys", rb_ary_sort(vsorted_keys));

  return Qtrue;
}

static VALUE cFastHashRing_gen_key(VALUE vself, VALUE vstring_key){
  unsigned int key = hash_val(RSTRING_PTR(vstring_key), 0);

  return INT2NUM(key);
}

int bisect(VALUE vsorted_keys, VALUE vkey){
  int i, vsorted_keys_len;

  vsorted_keys_len = RARRAY_LEN(vsorted_keys);
  for(i=0;i<vsorted_keys_len;i++){
    VALUE vsorted_key = rb_ary_entry(vsorted_keys, i);
    if (NUM2LONG(vkey) < NUM2LONG(vsorted_key)) return i;
  }

  return vsorted_keys_len;
}

static VALUE cFastHashRing_get_node_pos(VALUE vself, VALUE vstring_key){
  VALUE vring, vsorted_keys, vkey;
  int pos;

  vring = rb_iv_get(vself, "@ring");

  if(rb_funcall(vring, rb_intern("empty?"), 0)) return Qnil;

  vsorted_keys = rb_iv_get(vself, "@sorted_keys");

  vkey = cFastHashRing_gen_key(vself, vstring_key);
  pos = bisect(vsorted_keys, vkey);

  if(pos == RARRAY_LEN(vsorted_keys))
    return INT2NUM(0);
  else
    return INT2NUM(pos);
}

static VALUE cFastHashRing_get_node(VALUE vself, VALUE vstring_key){
  VALUE vpos, vring, vsorted_keys, vkey, vnode;
  vpos = cFastHashRing_get_node_pos(vself, vstring_key);

  if(vpos == Qnil) return Qnil;

  vring = rb_iv_get(vself, "@ring");
  vsorted_keys = rb_iv_get(vself, "@sorted_keys");

  vkey = rb_ary_entry(vsorted_keys, NUM2INT(vpos));
  vnode = rb_hash_aref(vring, vkey);

  return vnode;
}

static VALUE cFastHashRing_iterate_nodes(VALUE vself, VALUE vstring_key){
  VALUE vreturned_values, vpos, vring, vsorted_keys;
  int i;

  vreturned_values = rb_ary_new();
  vpos = cFastHashRing_get_node_pos(vself, vstring_key);

  vring = rb_iv_get(vself, "@ring");
  vsorted_keys = rb_iv_get(vself, "@sorted_keys");

  for(i=0;i<RARRAY_LEN(vsorted_keys);i++){
    VALUE vkey = rb_ary_entry(vsorted_keys, i);
    VALUE vvalue = rb_hash_aref(vring, vkey);
    if(!(rb_ary_includes(vreturned_values, vvalue)))
      rb_ary_push(vreturned_values, vvalue);
  }

  for(i=0;i<RARRAY_LEN(vsorted_keys);i++){
    if(i >= NUM2INT(vpos)) break;

    VALUE vkey = rb_ary_entry(vsorted_keys, i);
    VALUE vvalue = rb_hash_aref(vring, vkey);

    if(rb_ary_includes(vreturned_values, vvalue)) continue;

    rb_ary_push(vreturned_values, vvalue);
  }

  return vreturned_values;
}

static VALUE cFastHashRing_initialize(int argc, VALUE *argv, VALUE vself){
  VALUE vnodes, vweights, vring, vsorted_keys;

  rb_scan_args(argc, argv, "11", &vnodes, &vweights);

  if(TYPE(vnodes) != T_ARRAY) vnodes = rb_ary_new3(1, vnodes);
  Check_Type(vnodes, T_ARRAY);
  if(NIL_P(vweights)) vweights = rb_hash_new();
  vring = rb_hash_new();
  vsorted_keys = rb_ary_new();

  rb_iv_set(vself, "@nodes", vnodes);
  rb_iv_set(vself, "@weights", vweights);

  rb_iv_set(vself, "@ring", vring);
  rb_iv_set(vself, "@sorted_keys", vsorted_keys);

  return cFastHashRing_generate_circle(vself);
}

static VALUE cFastHashRing_sorted_keys(VALUE vself){
  return rb_iv_get(vself, "@sorted_keys");
}

static VALUE cFastHashRing_nodes(VALUE vself){
  return rb_iv_get(vself, "@nodes");
}

VALUE cFastHashRing;

void Init_fast_hash_ring(){
  cFastHashRing = rb_define_class("FastHashRing", rb_cObject);

  rb_define_private_method(cFastHashRing, "initialize", cFastHashRing_initialize, -1);
  rb_define_private_method(cFastHashRing, "generate_circle", cFastHashRing_generate_circle, -1);
  rb_define_method(cFastHashRing, "get_node", cFastHashRing_get_node, 1);
  rb_define_method(cFastHashRing, "get_node_pos", cFastHashRing_get_node_pos, 1);
  rb_define_method(cFastHashRing, "iterate_nodes", cFastHashRing_iterate_nodes, 1);
  rb_define_method(cFastHashRing, "gen_key", cFastHashRing_gen_key, 1);
  rb_define_method(cFastHashRing, "sorted_keys", cFastHashRing_sorted_keys, 0);
  rb_define_method(cFastHashRing, "nodes", cFastHashRing_nodes, 0);
}
