# I2C

## Topic: Start and Stop Conditions

## Date: 23/09/2025 

---

### Cue Column (Questions, Keywords, or Prompts)

---

### Notes Section (Main Notes)

**1. I2C Protocol without a clock stretching**
- Similar to UART, we need to send start and stop communication before we actually start communicating the data.
- So we have start condition which will wake up all the slave devices connected to the master, then we actually send the data.
- The data consists of both address as well as the actual data
- We already known I2C is a multi-master and multi-slave protocol, so we could have multiple slaves. To wake up a specific slave, we need to send the address of that slave.
  - We will first send the address of the slave we want to communicate with, then the specific slave will respond with an ACK bit, and then we can actually start transmitting the data. Once we complete our data transmission, we will send a stop condition to end the communication.
  - This is the typical format of the I2C bus
  - It could either be a read or write operation, so we will have a read/write bit after the address.
- **Start condition:**
  - SCL is high, SDA goes from high to low
- **Stop condition:**
  - SCL is high, SDA goes from low to high
- Of course, this should occur within a single bit duration. The simplest strategy is to divide an entire bit duration into 4 equal parts.
  - In the first part, we will keep both SCL and SDA high.
  - In the second part, we will pull down SDA to low while keeping SCL high. This is our start condition.
  - In the third part, we will pull down SCL to low while keeping SDA low. Now both SCL and SDA are low.
  - In the fourth part, we will keep SCL low and pull up SDA to high. Now SCL is low and SDA is high.
  - This completes one bit duration. We can repeat this process for the next bit.

![alt text](Screenshot-187.png)

---

### Summary Section (Summary of Notes)
