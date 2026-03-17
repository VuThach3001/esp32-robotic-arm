# I2C

## Topic: I2C Master without clock stretch

## Date: 26/09/2025 

---

### Cue Column (Questions, Keywords, or Prompts)

---

### Notes Section (Main Notes)

**1. Initiating Communication:**

- The lecture describes how to read data from a slave by setting a signal high to indicate readiness and then sampling the input data bus, which includes two important components: the slave address and the operation type (read or write).

**2. Communication Protocol:**
- After establishing the start condition, the master sends the slave address along with the operation type and the least significant bit (LSB) of the address.
- Following this, the master waits for an acknowledgment from the slave device. If the slave acknowledges by pulling the SDA line low, the communication proceeds; otherwise, the master sends a stop condition to gracefully terminate the interaction.

**3. Operations:**

**Write Operation:**
- This is indicated by an operation value of zero. The master transmits data to the slave and must again wait for an acknowledgment. If the slave acknowledges, the master concludes the communication with a stop bit and reverts to an idle state. If acknowledgment is not received, an acknowledgment error occurs, and the master returns to the start state.

**Read Operation:**
- For a read operation, indicated by an operation value of one, the master retrieves data from the slave. After the master receives 8 bits of data, it sends a negative acknowledgment to signal the end of data reception, followed by a stop condition to finalize the transaction.

---

### Summary Section (Summary of Notes)

- Each bit duration is divided into four pulses, allowing for precise transactions on both the S (start condition) and SDA lines.
- The open-drain concept is essential as it permits the line to either be pulled low or released, fostering effective communication.
- The lecture emphasizes the need for manual verification of operations by actively controlling the SDA line and suggests specific code modifications to monitor the SDA line's status throughout the operations.