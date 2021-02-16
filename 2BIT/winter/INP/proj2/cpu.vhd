-- cpu.vhd: Simple 8-bit CPU (BrainF*ck interpreter)
-- Copyright (C) 2019 Brno University of Technology,
--                    Faculty of Information Technology
-- Author(s): DOPLNIT
--

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

-- ----------------------------------------------------------------------------
--                        Entity declaration
-- ----------------------------------------------------------------------------
entity cpu is
 port (
   CLK   : in std_logic;  -- hodinovy signal
   RESET : in std_logic;  -- asynchronni reset procesoru
   EN    : in std_logic;  -- povoleni cinnosti procesoru
 
   -- synchronni pamet RAM
   DATA_ADDR  : out std_logic_vector(12 downto 0); -- adresa do pameti
   DATA_WDATA : out std_logic_vector(7 downto 0); -- mem[DATA_ADDR] <- DATA_WDATA pokud DATA_EN='1'
   DATA_RDATA : in std_logic_vector(7 downto 0);  -- DATA_RDATA <- ram[DATA_ADDR] pokud DATA_EN='1'
   DATA_RDWR  : out std_logic;                    -- cteni (0) / zapis (1)
   DATA_EN    : out std_logic;                    -- povoleni cinnosti
   
   -- vstupni port
   IN_DATA   : in std_logic_vector(7 downto 0);   -- IN_DATA <- stav klavesnice pokud IN_VLD='1' a IN_REQ='1'
   IN_VLD    : in std_logic;                      -- data platna
   IN_REQ    : out std_logic;                     -- pozadavek na vstup data
   
   -- vystupni port
   OUT_DATA : out  std_logic_vector(7 downto 0);  -- zapisovana data
   OUT_BUSY : in std_logic;                       -- LCD je zaneprazdnen (1), nelze zapisovat
   OUT_WE   : out std_logic                       -- LCD <- OUT_DATA pokud OUT_WE='1' a OUT_BUSY='0'
 );
end cpu;


-- ----------------------------------------------------------------------------
--                      Architecture declaration
-- ----------------------------------------------------------------------------
architecture behavioral of cpu is
	type states is (
      st_default,
		st_fetch, st_decode,
      st_ptr_inc, st_ptr_dec,
      st_inc_1, st_inc_2,
		st_dec_1, st_dec_2,
      st_putchar_1, st_putchar_2,
      st_getchar_1, st_getchar_2,
      st_while_s, st_while_s1, st_while_s2, st_while_s3,
      st_while_e, st_while_e1, st_while_e2, st_while_e3, st_while_e4, st_while_e5,
		st_load_tmp_1, st_load_tmp_2,
		st_store_tmp_1, st_store_tmp_2,
      st_halt,
      st_skip
   );
   signal current_state : states;
   signal next_state 	: states;
	
	type instructions is (
		inst_ptr_inc, inst_ptr_dec,
		inc, dec,
		while_start, while_end,
		putchar, getchar,
		load_tmp, store_tmp, 
		halt,
		not_inst
	);
	signal inst : instructions;

	signal pc_reg : std_logic_vector(12 downto 0);
	signal pc_inc  : std_logic;
	signal pc_dec  : std_logic;

	signal ptr_reg : std_logic_vector(12 downto 0);
	signal ptr_inc  : std_logic;
	signal ptr_dec  : std_logic;

	signal cnt_reg : std_logic_vector(7 downto 0);
	signal cnt_inc  : std_logic;
	signal cnt_dec  : std_logic;
	signal cnt_assign_one  : std_logic;

	signal sel1 : std_logic_vector(1 downto 0);
	signal sel2 : std_logic_vector(1 downto 0);

begin

	program_counter: process(CLK, RESET)
	begin
		if (RESET = '1') then
			pc_reg <= (others=>'0');		
		elsif rising_edge(CLK) then
			if (pc_inc = '1') then
				pc_reg <= pc_reg + 1;
			elsif (pc_dec = '1') then
				pc_reg <= pc_reg - 1;
			end if;
		end if;	
	end process; 

	pointer_register: process(CLK, RESET)
	begin
		if (RESET = '1') then
			ptr_reg <= "1000000000000";
		elsif rising_edge(CLK) then
			if (ptr_inc = '1') then
				if (ptr_reg = "1111111111111") then
					ptr_reg <= "1000000000000";
				else
					ptr_reg <= ptr_reg + 1;
				end if;
			elsif (ptr_dec = '1') then
				if (ptr_reg = "1000000000000") then
					ptr_reg <= "1111111111111";
				else
					ptr_reg <= ptr_reg - 1;
				end if;
			end if;
		end if;	
	end process; 

	counter_register: process(CLK, RESET)
	begin
		if (RESET  = '1') then
			cnt_reg <= (others=>'0');	
		elsif rising_edge(CLK) then
			if (cnt_inc = '1') then
				cnt_reg <= cnt_reg + 1;
			elsif (cnt_dec = '1') then
				cnt_reg <= cnt_reg - 1;
			elsif (cnt_assign_one = '1') then
				cnt_reg <= "00000001";
			end if;
		end if;	
	end process;

	decoder: process(DATA_RDATA)
	begin
		case (DATA_RDATA) is
			when X"3E" => inst <= inst_ptr_inc;  		-- >
			when X"3C" => inst <= inst_ptr_dec;  		-- <
			when X"2B" => inst <= inc;  					-- +
			when X"2D" => inst <= dec;  					-- -
			when X"5B" => inst <= while_start; 			-- [
			when X"5D" => inst <= while_end;	 		   -- ]
			when X"2E" => inst <= putchar;	   		-- .
			when X"2C" => inst <= getchar;			   -- ,
			when X"24" => inst <= load_tmp;				-- $
			when X"21" => inst <= store_tmp; 			-- !
			when X"00" => inst <= halt;  					-- null
			when others => inst <= not_inst; 			-- not an instruction
		end case;
	end process;

	multiplexor1: process(sel1, pc_reg, ptr_reg)
	begin
		case sel1 is
			when "00" => DATA_ADDR <= pc_reg;
			when "01" => DATA_ADDR <= ptr_reg;
			when "10" => DATA_ADDR <= "1000000000000";
			when others =>
		end case;
	end process;

	multiplexor2: process(sel2, DATA_RDATA, IN_DATA)
	begin
		case sel2 is
			when "00" => DATA_WDATA <= IN_DATA;
			when "01" => DATA_WDATA <= DATA_RDATA - 1;
			when "10" => DATA_WDATA <= DATA_RDATA + 1;
			when "11" => DATA_WDATA <= DATA_RDATA;
			when others =>
		end case;
	end process;

	state_handler: process(RESET, CLK, EN)
	begin
		if (RESET='1') then
			current_state <= st_default;
		elsif rising_edge(CLK) then
			if(EN = '1') then
				current_state <= next_state; 
			end if;
	  end if;
	end process;
	
	automat: process(DATA_RDATA, OUT_BUSY, IN_VLD, IN_DATA, current_state, inst, sel1, sel2, cnt_reg)
	begin
		DATA_EN	 <= '0';
		DATA_RDWR <= '0';
		IN_REQ	 <= '0';
		OUT_WE	 <= '0';
		sel2		 <= "00";
		sel1		 <= "00";
		ptr_dec	 <= '0';
		ptr_inc	 <= '0';
		pc_dec	 <= '0';
		pc_inc	 <= '0';
		cnt_inc	 <= '0';
		cnt_dec	 <= '0';
		cnt_assign_one <= '0';
		
		case current_state is
			when st_default =>
				next_state <= st_fetch;
			when st_fetch =>
				DATA_EN <= '1';
				sel1 <= "00";
				next_state <= st_decode;
			when st_decode =>
				case inst is
					when inst_ptr_inc =>
						next_state <= st_ptr_inc;
					when inst_ptr_dec =>
						next_state <= st_ptr_dec;
					when inc =>
						next_state <= st_inc_1;
					when dec =>
						next_state <= st_dec_1;
					when while_start =>
						next_state <= st_while_s;
					when while_end =>
						next_state <= st_while_e;
					when putchar =>
						next_state <= st_putchar_1;
					when getchar =>
						next_state <= st_getchar_1;
					when load_tmp =>
						next_state <= st_load_tmp_1;
					when store_tmp =>
						next_state <= st_store_tmp_1;
					when halt =>
						next_state <= st_halt;
					when others =>
						next_state <= st_skip;
				end case;
			when st_ptr_inc =>
				ptr_inc <= '1';
				pc_inc <= '1';
				next_state <= st_fetch;
			when st_ptr_dec =>
				ptr_dec <= '1';
				pc_inc <= '1';
				next_state <= st_fetch;
			when st_inc_1 =>
				DATA_EN <= '1';
				DATA_RDWR <= '0';
				sel1 <= "01";	
				next_state <= st_inc_2;
			when st_inc_2 =>
				DATA_EN <= '1';
				DATA_RDWR <= '1';
				sel1 <= "01";
				sel2 <= "10";
				pc_inc	<= '1';
				next_state <= st_fetch;
			when st_dec_1 =>
				DATA_EN <= '1';
				DATA_RDWR <= '0';
				sel1 <= "01";	
				next_state <= st_dec_2;
			when st_dec_2 =>
				DATA_EN <= '1';
				DATA_RDWR <= '1';
				sel1 <= "01";
				sel2 <= "01";
				pc_inc <= '1';
				next_state <= st_fetch;
			when st_halt =>
				next_state <= st_halt;
			when st_skip =>
				pc_inc <= '1';
				next_state <= st_fetch;
			when st_putchar_1 =>
				DATA_EN <= '1';
				DATA_RDWR <= '0';
				sel1 <= "01";
				next_state <= st_putchar_2;
			when st_putchar_2 =>
				if (OUT_BUSY = '0') then
					OUT_WE <= '1';
					OUT_DATA <= DATA_RDATA;
					pc_inc <= '1';
					next_state <= st_fetch;
				else
					next_state <= st_putchar_2;
				end if;
			when st_getchar_1 =>
				IN_REQ <= '1';
				next_state <= st_getchar_2;
			when st_getchar_2 =>
				if (IN_VLD = '1') then
					DATA_EN <= '1';
					DATA_RDWR <= '1';
					sel1 <= "01";
					sel2 <= "00";
					pc_inc <= '1';
					next_state <= st_fetch;
				else
					IN_REQ <= '1';
					next_state <= st_getchar_2;
				end if;
			when st_load_tmp_1 =>
				DATA_EN <= '1';
				DATA_RDWR <= '0';
				sel1 <= "01";
				next_state <= st_load_tmp_2;
			when st_load_tmp_2 =>
				DATA_EN <= '1';
				DATA_RDWR <= '1';
				sel1 <= "10";
				sel2 <= "11";
				pc_inc <= '1';
				next_state <= st_fetch;
			when st_store_tmp_1 =>
				DATA_EN <= '1';
				DATA_RDWR <= '0';
				sel1 <= "10";
				next_state <= st_store_tmp_2;
			when st_store_tmp_2 =>
				DATA_EN <= '1';
				DATA_RDWR <= '1';
				sel1 <= "01";
				sel2 <= "11";
				pc_inc <= '1';
				next_state <= st_fetch;
			when st_while_s =>
				DATA_EN <= '1';
				DATA_RDWR <= '0';
				sel1 <= "01";
				pc_inc <= '1';
				next_state <= st_while_s1;
			when st_while_s1 =>
				if (DATA_RDATA = "00000000") then
					cnt_assign_one <= '1';
					next_state <= st_while_s2;
				else
					next_state <= st_fetch;
				end if;
			when st_while_s2 =>
				if (cnt_reg = "00000000") then
					next_state <= st_fetch;
				else
					DATA_EN <= '1';
					DATA_RDWR <= '0';
					sel1 <= "00";
					next_state <= st_while_s3;
				end if;
			when st_while_s3 =>
				if(inst = while_start) then
					cnt_inc <= '1';
				elsif(inst = while_end) then
					cnt_dec <= '1';
				end if;
				pc_inc <= '1';
				next_state <= st_while_s2;
			when st_while_e =>
				DATA_EN <= '1';
				DATA_RDWR <= '0';
				sel1 <= "01";
				next_state <= st_while_e1;
			when st_while_e1 =>
				if(DATA_RDATA = "00000000") then
					pc_inc <= '1';
					next_state <= st_fetch;
				else
					cnt_assign_one <= '1';
					pc_dec <= '1';
					next_state <= st_while_e3;
				end if;
			when st_while_e2 =>
				
			when st_while_e3 =>
				if(cnt_reg = "00000000") then
					next_state <= st_fetch;
				else
					DATA_EN <= '1';
					DATA_RDWR <= '0';
					sel1 <= "00";
					next_state <= st_while_e4;
				end if;
			when st_while_e4 =>
				if(inst = while_start) then
					cnt_dec <= '1';
				elsif(inst = while_end) then
					cnt_inc <= '1';
				end if;
				next_state <= st_while_e5;
			when st_while_e5 =>
				if(cnt_reg = "00000000") then
					pc_inc <= '1';
				else
					pc_dec <= '1';
				end if;
				next_state <=st_while_e3;
			when others =>
		end case;
			
			
	end process;
end behavioral;
 
