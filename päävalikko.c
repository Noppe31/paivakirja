#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_MERKINNAT 100
#define MAX_PITUUS 256

typedef struct {
    char teksti[MAX_PITUUS];
} Merkinta;

static void lue_rivi(const char *kehote, char *kohde, size_t koko) {
    printf("%s", kehote);

    if (fgets(kohde, (int)koko, stdin) == NULL) {
        kohde[0] = '\0';
        return;
    }

    kohde[strcspn(kohde, "\n")] = '\0';
}

static int lue_numero(const char *kehote) {
    char syote[32];
    char *loppu;
    long numero;

    lue_rivi(kehote, syote, sizeof syote);
    numero = strtol(syote, &loppu, 10);

    if (loppu == syote || *loppu != '\0') {
        return -1;
    }

    return (int)numero;
}

static void listaa_merkinnat(const Merkinta merkinnat[], int maara) {
    if (maara == 0) {
        printf("Ei merkintoja.\n");
        return;
    }

    printf("\n--- Merkinnat ---\n");
    for (int i = 0; i < maara; i++) {
        printf("%d. %s\n", i + 1, merkinnat[i].teksti);
    }
}

static void lisaa_merkinta(Merkinta merkinnat[], int *maara) {
    if (*maara >= MAX_MERKINNAT) {
        printf("Merkintojen enimm ais maara on saavutettu.\n");
        return;
    }

    lue_rivi("Kirjoita merkinta: ", merkinnat[*maara].teksti, MAX_PITUUS);
    if (merkinnat[*maara].teksti[0] == '\0') {
        printf("Tyhjaa merkintaa ei lisatty.\n");
        return;
    }

    (*maara)++;
    printf("Merkinta lisatty.\n");
}

static int valitse_merkinta(const Merkinta merkinnat[], int maara) {
    int numero;

    listaa_merkinnat(merkinnat, maara);
    if (maara == 0) {
        return -1;
    }

    numero = lue_numero("Valitse merkinnan numero: ");
    if (numero < 1 || numero > maara) {
        printf("Virheellinen merkinnan numero.\n");
        return -1;
    }

    return numero - 1;
}

static void muokkaa_merkintaa(Merkinta merkinnat[], int maara) {
    int indeksi = valitse_merkinta(merkinnat, maara);
    if (indeksi < 0) {
        return;
    }

    lue_rivi("Kirjoita uusi teksti: ", merkinnat[indeksi].teksti, MAX_PITUUS);
    printf("Merkinta muokattu.\n");
}

static void poista_merkinta(Merkinta merkinnat[], int *maara) {
    int indeksi = valitse_merkinta(merkinnat, *maara);
    if (indeksi < 0) {
        return;
    }

    for (int i = indeksi; i < *maara - 1; i++) {
        merkinnat[i] = merkinnat[i + 1];
    }

    (*maara)--;
    printf("Merkinta poistettu.\n");
}

int main(void) {
    Merkinta merkinnat[MAX_MERKINNAT];
    int maara = 0;
    int valinta;

    do {
        printf("\n=== Paivakirja ===\n");
        printf("1. Lisaa merkinta\n");
        printf("2. Tarkastele / listaa merkinnat\n");
        printf("3. Muokkaa merkintaa\n");
        printf("4. Poista merkinta\n");
        printf("5. Lopeta\n");

        valinta = lue_numero("Valitse toiminto (1-5): ");

        switch (valinta) {
            case 1:
                lisaa_merkinta(merkinnat, &maara);
                break;
            case 2:
                listaa_merkinnat(merkinnat, maara);
                break;
            case 3:
                muokkaa_merkintaa(merkinnat, maara);
                break;
            case 4:
                poista_merkinta(merkinnat, &maara);
                break;
            case 5:
                printf("Ohjelma suljetaan.\n");
                break;
            default:
                printf("Virheellinen valinta. Valitse numero 1-5.\n");
                break;
        }
    } while (valinta != 5);

    return 0;
}
