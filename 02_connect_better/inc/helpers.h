#define PORT "2019"
#define FATAL_ERROR(str) { perror("[-] FATAL_ERROR at: "str); exit(EXIT_FAILURE); }
#define NAME_LOOKUP_ERROR(status) { fprintf(stderr, "[-] FATAL_ERROR at: NAME_RESOLUTION: %s", gai_strerror(status)); exit(EXIT_FAILURE); }
