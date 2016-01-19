#include <stdint.h>
#include <stdio.h>

#define NUM_TRANSIZIONI (/*NUM_TRANSIZIONI*/)
#define NUM_INGRESSI    (/*NUM_INGRESSI*/)
#define BIT(n)          (1 << n)
#define NTH_BIT(x, n)   ((BIT(n) & x) >> n)

typedef uint32_t io_t;
typedef int      stato_t;

stato_t	stato;
io_t	raw_inputs, inputs, outputs, bus;

// Gli stati di partenza
stato_t partenza[] = {/*PARTENZA*/};

// Gli stati di arrivo
stato_t arrivo[] = {/*ARRIVO*/};

// Gli ingressi fisici considerati per una transizione
io_t port_in_bitmask[] = {/*PORT_IN_BITMASK*/};
// Gli ingressi di bus considerati per una transizione
io_t bus_in_bitmask[] =  {/*BUS_IN_BITMASK*/};

// I valori attesi degli ingressi fisici perche' si faccia la transizione
io_t port_in_valori[] = {/*PORT_IN_VALORI*/};
// I valori attesi degli ingressi di bus perche' si faccia la transizione
io_t bus_in_valori[] = {/*BUS_IN_VALORI*/};

// Le uscite fisiche modificate da una transizione
io_t port_out_bitmask[] = {/*PORT_OUT_BITMASK*/};
// Le uscite di bus modificate da una transizione
io_t bus_out_bitmask[] = {/*BUS_OUT_BITMASK*/};

// I valori scritti su una porta fisica da una transizione
io_t port_out_valori[] = {/*PORT_OUT_VALORI*/};
// I valori scritti su una porta virtuale da una transizione
io_t bus_out_valori[] = {/*BUS_OUT_VALORI*/};

// ---

uint8_t debounced[] = {/*INGRESSI_DEBOUNCED*/};

uint8_t input_counts[] = {/*INPUT_COUNTS*/};

void init() {
	stato = /*STATO_INIZIALE*/;
	raw_inputs = inputs = /*INPUTS_INIZIALI*/; // Evita di dover aspettare il debounce
	return;
}

int main() {
	int i, counter = 0;

	init();

// Il loop inizia da qua

	// raw_inputs = PORTA

	printf("Stato iniziale: %i\n", stato);

	counter++;
	counter %= /*INTERVALLO*/;

	if (counter == 0) {
		for (i = 0; i < NUM_INGRESSI; i++) {
			#define ingresso ( !!( raw_inputs & (1 << i) ) )
			#define count (input_counts[i])

			if (!debounced[i]) {
				if (ingresso)
					inputs |=  (1 << i); // Set
				else
					inputs &= ~(1 << i); // Clear
				continue;
			}

			if (ingresso && count != 0xFF)
				count++;
			else if (!ingresso && count != 0x00)
				count--;

			// Trigger di Schmitt
			if ((ingresso && count < /*SOGLIA_BASSA*/) || (!ingresso && count > /*SOGLIA_ALTA*/))
				inputs ^= BIT(i); // Toggle
		}
	}

	for (i = 0; i < NUM_TRANSIZIONI; i++) {
		// stato non corrispondente? passa al prossimo
		if (stato != partenza[i]) continue;
		// porte fisiche di ingresso non corrispondenti? passa
		if ((inputs & port_in_bitmask[i]) != port_in_valori[i]) continue;
		// porte virtuali di ingresso non corrispondenti? passa
		if ((bus & bus_in_bitmask[i]) != bus_in_bitmask[i]) continue;
		// se sei qua, lo stato e le condizioni corrispondono
		stato = arrivo[i];

		// Clear
		outputs &= ~port_out_bitmask[i];
		// Set
		outputs |= port_out_valori[i];

		// Clear
		bus &= ~bus_out_bitmask[i];
		// Set
		bus |= bus_out_valori[i];
		break;
	}
	// Verifica che la transizione e' avvenuta
	printf("Stato finale: %i, output finali: %i\n", stato, outputs);

// e finisce qua

	return 0;
}