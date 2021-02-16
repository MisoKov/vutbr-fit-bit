-- Autor reseni: Michal Koval, xkoval17

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_arith.all;
use IEEE.std_logic_unsigned.all;

entity ledc8x8 is
port (
    RESET : IN std_logic;
    SMCLK : IN std_logic;
    ROW : OUT std_logic_vector(0 to 7);
    LED : OUT std_logic_vector(0 to 7)

);
end ledc8x8;

architecture main of ledc8x8 is

    signal ce_counter : std_logic_vector(11 downto 0) := (others => '0');
    signal change_state : std_logic_vector(10 downto 0) := (others => '0');
    signal state : std_logic  := '0';
    signal last_state : std_logic := '0';
    signal enable : std_logic  := '0';
    signal led_out : std_logic_vector(7 downto 0) := (others => '0');
    signal row_out : std_logic_vector(7 downto 0) := (others => '0');


begin
    ---------------------------------
	-- Citac na znizenie frekvencie--
	---------------------------------

    smclk_div: process(SMCLK, RESET)
    begin
		if RESET = '1' then
			ce_counter <= (others => '0');
      elsif rising_edge(SMCLK) then
			if ce_counter = "111000010000" then
				enable <= '1';
            ce_counter <= (others => '0');
         else
				enable <= '0';
         end if;
         ce_counter <= ce_counter + 1;
      end if;
    end process smclk_div;

    ---------------------------------
	-------Rotacia riadkov ----------
	---------------------------------

   row_rot: process(RESET, SMCLK, enable)
	begin
		if RESET = '1' then
			row_out <= "10000000";
		elsif rising_edge(SMCLK) and enable = '1' then
			row_out <= row_out(0) & row_out(7 downto 1);
		end if;
	end process row_rot;

    ---------------------------------
	------- Zmena stavu ------------
	---------------------------------

    state_changer: process(SMCLK, RESET, enable)
    begin
        if RESET = '1' then
            change_state <= (others => '0');
        elsif rising_edge(SMCLK) and enable = '1' then
            if change_state = "10000000000" then
					if last_state = '0' then
						state <= '1';
					else
						state <= '0';
					end if;
					change_state <=  (others => '0');
            else
					change_state <= change_state + 1;
				end if;
        end if;
    end process state_changer;


	---------------------------------
	--------Dekoder riadkov----------
	---------------------------------
	decoder: process(row_out, state)
	begin
	    if state = '0' then
            case row_out is
                when "10000000" => led_out <= "01110111";
                when "01000000" => led_out <= "00100111";
                when "00100000" => led_out <= "01010111";
                when "00010000" => led_out <= "01110110";
                when "00001000" => led_out <= "01110101";
                when "00000100" => led_out <= "11110011";
                when "00000010" => led_out <= "11110101";
                when "00000001" => led_out <= "11110110";
                when others =>     led_out <= "11111111";
            end case;
		else
            case row_out is
                when "10000000" => led_out <= "11111111";
                when "01000000" => led_out <= "11111111";
                when "00100000" => led_out <= "11111111";
                when "00010000" => led_out <= "11111111";
                when "00001000" => led_out <= "11111111";
                when "00000100" => led_out <= "11111111";
                when "00000010" => led_out <= "11111111";
                when "00000001" => led_out <= "11111111";
                when others =>     led_out <= "11111111";
            end case;
				last_state <= '1';
        end if;
	end process decoder;

	---------------------------------
	--------Zasvietenie riadkov------
	---------------------------------
	ROW <= row_out;
	LED <= led_out;

end main;

-- ISID: 75579
