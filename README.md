### Architecture Overview
```text          
      +-----------------------+
      |     Button Task       |
      |-----------------------|
      | - Read button state   |
      +-----------------------+
                  |
                  v
      +-----------------------+
      |     Button Queue      |
      +-----------------------+
                  |
                  v
      +-----------------------+
      |     State Machine     |
      |         Task          |
      |-----------------------|
      | - Wait for event      |
      | - Update state        |
      | - Retrive state data  |
      | - Scale and decode    |
      | - Send data to LCD    |
      +-----------------------+
                  |
                  v
      +-----------------------+
      |      LCD Queue        |
      +-----------------------+
                  |
                  v
      +-----------------------+
      |       LCD Task        |
      |-----------------------|
      | - Display state data  |
      +-----------------------+
```

### Subaru Select Monitor 1 Communication protocol
```text
| Command                | Description |
|------------------------|-------------|
| 12    00    00    00   | stop read   |
| 78    msb   lsb   00   | read data   |
| AA    msb   lsb   data | write data  |
| 00    46    48    49   | get rom id  |
```