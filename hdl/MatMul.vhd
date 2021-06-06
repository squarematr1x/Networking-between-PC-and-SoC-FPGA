--Generated with SaturaViisari (see: https://lipas.uwasa.fi/~TAU/ICAT1040/Slides/js/SaturaViisari.html)

library IEEE;
use IEEE.STD_LOGIC_1164.all;
use IEEE.NUMERIC_STD.all;
--use IEEE.STD_LOGIC_arith.all;
--
entity TOmultiplier_p1_p0_p0 is
    port (
      x : in std_logic_vector (7 downto 0);
      xC1,xC2,xC3 : out std_logic_vector (23 downto 0)
    );
end TOmultiplier_p1_p0_p0;
--
architecture struct of TOmultiplier_p1_p0_p0 is
    -- signal declarations
    signal R1,R2,R3,p,Zero: signed(32 downto 0):=(others => '0');
    function SignN(s : std_logic; n : integer) return signed is
        variable Zeros : signed(n-1 downto 0) := (others => '0');
        variable Ones : signed(n-1 downto 0) := (others => '1');
    begin
        if s='1' then
            return Ones;
        else
            return Zeros;
        end if;
    end SignN;
begin -- struct
    -- signal assignments
    p <= signed(SignN(x(7),25)& signed(x(7 downto 0))); -- p = x: adjust length of x
    --     xC1=x;
    R1 <= signed(SignN(p(7),25)& p(7 downto 0) );
    --     xC2=Zero;
    R2 <= Zero;
    --     xC3=Zero;
    R3 <= Zero;
    -- take the least significant bits of Ri:
    xC1 <= std_logic_vector(R1(23 downto 0));
    xC2 <= std_logic_vector(R2(23 downto 0));
    xC3 <= std_logic_vector(R3(23 downto 0));
end struct;
-- Number of adders: 0
library IEEE;
use IEEE.STD_LOGIC_1164.all;
use IEEE.NUMERIC_STD.all;
--use IEEE.STD_LOGIC_arith.all;
--
entity TOmultiplier_p0_p2_p0 is
    port (
      x : in std_logic_vector (7 downto 0);
      xC1,xC2,xC3 : out std_logic_vector (23 downto 0)
    );
end TOmultiplier_p0_p2_p0;
--
architecture struct of TOmultiplier_p0_p2_p0 is
    -- signal declarations
    signal R1,R2,R3,p,Zero: signed(32 downto 0):=(others => '0');
    function SignN(s : std_logic; n : integer) return signed is
        variable Zeros : signed(n-1 downto 0) := (others => '0');
        variable Ones : signed(n-1 downto 0) := (others => '1');
    begin
        if s='1' then
            return Ones;
        else
            return Zeros;
        end if;
    end SignN;
begin -- struct
    -- signal assignments
    p <= signed(SignN(x(7),25)& signed(x(7 downto 0))); -- p = x: adjust length of x
    --     xC1=Zero;
    R1 <= Zero;
    --     xC2=(x<<1);
    R2 <= signed(SignN(p(7),24)& p(7 downto 0) &b"0");
    --     xC3=Zero;
    R3 <= Zero;
    -- take the least significant bits of Ri:
    xC1 <= std_logic_vector(R1(23 downto 0));
    xC2 <= std_logic_vector(R2(23 downto 0));
    xC3 <= std_logic_vector(R3(23 downto 0));
end struct;
-- Number of adders: 0
library IEEE;
use IEEE.STD_LOGIC_1164.all;
use IEEE.NUMERIC_STD.all;
--use IEEE.STD_LOGIC_arith.all;
--
entity TOmultiplier_p0_p0_p3 is
    port (
      x : in std_logic_vector (7 downto 0);
      xC1,xC2,xC3 : out std_logic_vector (23 downto 0)
    );
end TOmultiplier_p0_p0_p3;
--
architecture struct of TOmultiplier_p0_p0_p3 is
    -- signal declarations
    signal R1,R2,R3,p,Zero: signed(32 downto 0):=(others => '0');
    function SignN(s : std_logic; n : integer) return signed is
        variable Zeros : signed(n-1 downto 0) := (others => '0');
        variable Ones : signed(n-1 downto 0) := (others => '1');
    begin
        if s='1' then
            return Ones;
        else
            return Zeros;
        end if;
    end SignN;
begin -- struct
    -- signal assignments
    p <= signed(SignN(x(7),25)& signed(x(7 downto 0))); -- p = x: adjust length of x
    --     xC1=Zero;
    R1 <= Zero;
    --     xC2=Zero;
    R2 <= Zero;
    --     xC3=(x<<1)+x;
    R3 <= signed(SignN(p(7),24)& p(7 downto 0) &b"0")
    +signed(SignN(p(7),25)& p(7 downto 0) );
    -- take the least significant bits of Ri:
    xC1 <= std_logic_vector(R1(23 downto 0));
    xC2 <= std_logic_vector(R2(23 downto 0));
    xC3 <= std_logic_vector(R3(23 downto 0));
end struct;
-- Number of adders: 1
    library IEEE;
    use IEEE.STD_LOGIC_1164.all;
    use IEEE.NUMERIC_STD.all;
    --
    entity MatrixMult is
      port (
        x,y,z : in std_logic_vector (7 downto 0);
        R1,R2,R3 : out std_logic_vector (23 downto 0)
      );
    end MatrixMult;
    architecture struct of MatrixMult is
    -- component declaration(s):
      component TOmultiplier_p1_p0_p0 is
        port (
          x : in std_logic_vector (7 downto 0);
          xC1,xC2,xC3 : out std_logic_vector (23 downto 0)
        );
      end component TOmultiplier_p1_p0_p0;
      component TOmultiplier_p0_p2_p0 is
        port (
          x : in std_logic_vector (7 downto 0);
          xC1,xC2,xC3 : out std_logic_vector (23 downto 0)
        );
      end component TOmultiplier_p0_p2_p0;
      component TOmultiplier_p0_p0_p3 is
        port (
          x : in std_logic_vector (7 downto 0);
          xC1,xC2,xC3 : out std_logic_vector (23 downto 0)
        );
      end component TOmultiplier_p0_p0_p3;
      -- signal declarations:
      signal xC1,xC2,xC3 : std_logic_vector (23 downto 0); -- outputs
      signal yB1,yB2,yB3 : std_logic_vector (23 downto 0); -- outputs
      signal zA1,zA2,zA3 : std_logic_vector (23 downto 0); -- outputs
      signal Zero : signed(23 downto 0):=(others => '0');
      begin -- struct
      -- connecting signals to multipliers:
      M1: TOmultiplier_p1_p0_p0 port map (x => x, xC1 => xC1, xC2 => xC2, xC3 => xC3);
      M2: TOmultiplier_p0_p2_p0 port map (x => y, xC1 => yB1, xC2 => yB2, xC3 => yB3);
      M3: TOmultiplier_p0_p0_p3 port map (x => z, xC1 => zA1, xC2 => zA2, xC3 => zA3);
      -- adders for dot products:
          -- constants: 1,0,0
      R1 <= std_logic_vector(signed(xC1));
          -- constants: 0,2,0
      R2 <= std_logic_vector(signed(yB2));
          -- constants: 0,0,3
      R3 <= std_logic_vector(signed(zA3));
    end struct;
