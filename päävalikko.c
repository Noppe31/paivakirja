#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_MERKINNAT 100
#define MAX_PITUUS 256
#define TALLENNUSTIEDOSTO "merkinnat.dat"

typedef struct {
    char teksti[MAX_PITUUS];
    char paivamaara[11];
} Merkinta;

static void lue_rivi(const char *kehote, char *kohde, size_t koko) {
    printf("%s", kehote);

    // fgets estää liian pitkän syötteen kirjoittamisen taulukon yli.
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

    // Loppuun jäävä teksti tarkoittaa, ettei syöte ollut kokonainen numero.
    if (loppu == syote || *loppu != '\0') {
        return -1;
    }

    return (int)numero;
}

static void aseta_paivamaara(char paivamaara[11]) {
    time_t nyt = time(NULL);
    struct tm *paikallinen_aika = localtime(&nyt);

    // Jos kellonaikaa ei saada, käytetään tyhjää päivämäärää.
    if (paikallinen_aika == NULL) {
        strcpy(paivamaara, "0000-00-00");
        return;
    }

    strftime(paivamaara, 11, "%Y-%m-%d", paikallinen_aika);
}

static int lataa_merkinnat(Merkinta merkinnat[], int *maara) {
    FILE *tiedosto = fopen(TALLENNUSTIEDOSTO, "rb");

    // Ensimmäisellä käynnistyskerralla tallennustiedostoa ei vielä ole.
    if (tiedosto == NULL) {
        return 1;
    }

    *maara = (int)fread(merkinnat, sizeof(Merkinta), MAX_MERKINNAT, tiedosto);
    fclose(tiedosto);
    return 1;
}

static int tallenna_merkinnat(const Merkinta merkinnat[], int maara) {
    FILE *tiedosto = fopen(TALLENNUSTIEDOSTO, "wb");
    if (tiedosto == NULL) {
        return 0;
    }

    size_t tallennetut = fwrite(merkinnat, sizeof(Merkinta), (size_t)maara, tiedosto);
    fclose(tiedosto);
    return tallennetut == (size_t)maara;
}

static void listaa_merkinnat(const Merkinta merkinnat[], int maara) {
    if (maara == 0) {
        printf("Ei merkintoja.\n");
        return;
    }

    printf("\n--- Merkinnat ---\n");
    // Samassa näkymässä näkyvät sekä teksti että sen luontipäivä.
    for (int i = 0; i < maara; i++) {
        printf("%d. [%s] %s\n", i + 1, merkinnat[i].paivamaara, merkinnat[i].teksti);
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

    aseta_paivamaara(merkinnat[*maara].paivamaara);
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
    // Käyttäjälle näytetään numerot alkaen yhdestä, taulukossa indeksit alkavat nollasta.
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

    // Muokatessa päivämäärä jätetään ennalleen, koska merkintä on tehty aiemmin.
    lue_rivi("Kirjoita uusi teksti: ", merkinnat[indeksi].teksti, MAX_PITUUS);
    printf("Merkinta muokattu.\n");
}

static void poista_merkinta(Merkinta merkinnat[], int *maara) {
    int indeksi = valitse_merkinta(merkinnat, *maara);
    if (indeksi < 0) {
        return;
    }

    // Poiston jälkeen seuraavat merkinnät siirretään yhden paikan vasemmalle.
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

    // Aiemmin tehdyt merkinnät haetaan heti ohjelman alussa.
    lataa_merkinnat(merkinnat, &maara);

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
                // Tiedot kirjoitetaan levylle ennen ohjelman sulkemista.
                if (!tallenna_merkinnat(merkinnat, maara)) {
                    printf("Merkintojen tallennus epaonnistui.\n");
                }
                printf("Ohjelma suljetaan.\n");
                break;
            default:
                printf("Virheellinen valinta. Valitse numero 1-5.\n");
                break;
        }
    } while (valinta != 5);

    return 0;
}
