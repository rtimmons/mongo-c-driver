#include <mongoc.h>

int
run_example(bson_t* pipeline)
{
   const bson_t *doc;
   bson_t *to_insert = BCON_NEW ("x", BCON_INT32 (1));
   const bson_t *err_doc;
   bson_error_t err;
   mongoc_client_t *client;
   mongoc_collection_t *coll;
   mongoc_change_stream_t *stream;
   mongoc_write_concern_t *wc = mongoc_write_concern_new ();
   bson_t opts = BSON_INITIALIZER;
   bool r;

   mongoc_init ();

   client = mongoc_client_new ("mongodb://"
                               "localhost:27017,localhost:27018"
                               "/db?replicaSet=replset");
   if (!client) {
      fprintf (stderr, "Could not connect to replica set\n");
      return 1;
   }

   coll = mongoc_client_get_collection (client, "db", "coll");
   stream = mongoc_collection_watch (coll, pipeline, NULL);

   mongoc_write_concern_set_wmajority (wc, 10000);
   mongoc_write_concern_append (wc, &opts);
   r = mongoc_collection_insert_one (coll, to_insert, &opts, NULL, &err);
   if (!r) {
      fprintf (stderr, "Error: %s\n", err.message);
      return 1;
   }

   while (mongoc_change_stream_next (stream, &doc)) {
      char *pipe_json = bson_as_relaxed_extended_json (pipeline, NULL);
      fprintf (stderr, "Pipeline: %s\n", pipe_json);
      bson_free (pipe_json);

      char *as_json = bson_as_relaxed_extended_json (doc, NULL);
      fprintf (stderr, "Got document: %s\n", as_json);
      bson_free (as_json);
   }

   if (mongoc_change_stream_error_document (stream, &err, &err_doc)) {
      if (!bson_empty (err_doc)) {
         fprintf (stderr,
                  "Server Error: %s\n",
                  bson_as_relaxed_extended_json (err_doc, NULL));
      } else {
         fprintf (stderr, "Client Error: %s\n", err.message);
      }
      return 1;
   }

   bson_destroy (to_insert);
   mongoc_write_concern_destroy (wc);
   bson_destroy (&opts);
   mongoc_change_stream_destroy (stream);
   mongoc_collection_destroy (coll);
   mongoc_client_destroy (client);
   mongoc_cleanup ();
}

int
example_one ()
{
   bson_t pipeline = BSON_INITIALIZER;
   return run_example(&pipeline);
}


int
example_two()
{
   bson_t* pipeline = BCON_NEW ("$match", "{",
                                "operationType", "update", "}");
   return run_example(pipeline);
}

int
main()
{
//   int one = example_one();
   int two = example_two();

//   return two;
return 0;
}