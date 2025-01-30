

void lcd_set_cursor(uint8_t row, uint8_t col) {
    uint8_t pos;
    if (row == 1) {
        pos = 0x80 + (col - 1);
    } else {
        pos = 0xC0 + (col - 1);
    }
    lcd_command(pos);
}
//call lcd_data() after lcd_set_cursor to write in LCD