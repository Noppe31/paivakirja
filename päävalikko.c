#include <stdio.h>

int main(void) {
    int valinta;

    do {
        printf("\n=== Paivakirja ===\n");
        printf("1. Lisaa merkinta\n");
        printf("2. Tarkastele / listaa merkinnat\n");
        printf("3. Muokkaa merkintaa\n");
        printf("4. Poista merkinta\n");
        printf("5. Lopeta\n");
        printf("Valitse toiminto (1-5): ");

        if (scanf("%d", &valinta) != 1) {
            printf("Virheellinen valinta. Anna numero 1-5.\n");

            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF) {
            }

            valinta = 0;
            continue;
        }

        switch (valinta) {
            case 1:
                printf("Valitsit: Lisaa merkinta.\n");
                break;
            case 2:
                printf("Valitsit: Tarkastele/listaa merkinnat.\n");
                break;
            case 3:
                printf("Valitsit: Muokkaa merkintaa.\n");
                break;
            case 4:
                printf("Valitsit: Poista merkinta.\n");
                break;
            case 5:
                printf("Ohjelma suljetaan.\n");
                break;
            default:
                printf("Virheellinen valinta. Valitse numero 1-5.\n");
                break;
        }

        if (valinta != 5) {
            printf("Palaa paavalikkoon.\n");
        }

    } while (valinta != 5);

    return 0;
}
