#pragma once

#include "includes.h"

#define TIMEOUT_SECONDS 5

struct Data
{
  char *data;
  size_t size;
};

int makePostRequest(char *, struct Data *);
static size_t requestCallback(char *, size_t, size_t, void *);
static size_t responseCallback(void *, size_t, size_t, void *);

int makePostRequest(char *url, struct Data *requestData)
{
  struct Data *chunk = calloc(1, sizeof(struct Data));

  CURLcode res;
  CURL *curl = curl_easy_init();

  void *requestHead = requestData->data;

  if (curl)
  {
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, TIMEOUT_SECONDS);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, TIMEOUT_SECONDS);

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_POST, 1L);

    curl_easy_setopt(curl, CURLOPT_READDATA, requestData);
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, requestCallback);

    curl_easy_setopt(curl, CURLOPT_WRITEDATA, chunk);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, responseCallback);

    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    curl_global_cleanup();
  }

  free(chunk->data);
  free(chunk);
  free(requestHead);
  free(requestData);

  return 0;
}

static size_t requestCallback(char *dest, size_t size, size_t nmemb, void *request)
{
  struct Data *requestData = (struct Data *)request;
  size_t bufferSize = size * nmemb;

  if (requestData->size)
  {
    size_t chunkSize = requestData->size;

    if (chunkSize > bufferSize)
    {
      chunkSize = bufferSize;
    }

    memcpy(dest, requestData->data, chunkSize);

    requestData->data += chunkSize;
    requestData->size -= chunkSize;

    return chunkSize;
  }

  return 0;
}

static size_t responseCallback(void *data, size_t size, size_t nmemb, void *response)
{
  size_t bufferSize = size * nmemb;
  struct Data *responseData = (struct Data *)response;

  char *ptr = realloc(responseData->data, responseData->size + bufferSize + 1);
  if (!ptr)
  {
    return 0;
  }

  responseData->data = ptr;
  memcpy(&(responseData->data[responseData->size]), data, bufferSize);
  responseData->size += bufferSize;
  responseData->data[responseData->size] = 0;

  // Uncomment following line for debugging
  printf("Reponse: %s\n", responseData->data);

  return bufferSize;
}
