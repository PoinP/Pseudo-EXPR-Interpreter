#include "../src/BigInteger/BigInteger.h"
#include <vector>
#include <random>
#include <string>

#include "doctest.h"

TEST_CASE("Constructors")
{
    SUBCASE("Default Ctor")
    {
        BigInteger bigInt;
        CHECK(bigInt == "0");
    }

    SUBCASE("Number Ctor")
    {
        BigInteger bigInt(10027738872U);
        CHECK(bigInt == "10027738872");
        bigInt = 9982;
        CHECK(bigInt == "9982");

        CHECK_THROWS(bigInt = -123);
    }

    SUBCASE("String Ctor")
    {
        BigInteger bigInt(std::string("8662553444667812830000001732981893122346237823437289"));
        CHECK((std::string)bigInt == std::string("8662553444667812830000001732981893122346237823437289"));

        CHECK_THROWS(bigInt = std::string("12ys"));
    }

    SUBCASE("Char Arr Ctor")
    {
        BigInteger bigInt = "2883777289";
        CHECK(bigInt == "2883777289");
        bigInt = "838327833248842387483297489239843288432";
        CHECK(bigInt == "838327833248842387483297489239843288432");
        CHECK_THROWS(bigInt = "12ys");
    }
}

TEST_CASE("Increment And Decrement Operators")
{
    BigInteger num1 = 123;
    CHECK(num1++ == 123);
    CHECK(++num1 == 125);
    CHECK(num1-- == 125);
    CHECK(--num1 == 123);
}

TEST_CASE("Self Assigning Operators")
{
    BigInteger num1 = 100;
    BigInteger num2 = 50;

    num1 += num2;
    CHECK(num1 == 150);
    num1 -= num2;
    CHECK(num1 == 100);
    num1 *= num2;
    CHECK(num1 == 5'000);
    num1 /= num2;
    CHECK(num1 == 100);
    num1 %= num2;
    CHECK(num1 == 0);
}

TEST_CASE("Relational operators")
{
    SUBCASE("With strings as input")
    {
        std::vector<BigInteger> bigNums =
        {
            BigInteger("0"),
            BigInteger("2182737621234"),
            BigInteger("435526788322937"),
            BigInteger("455637772883892"),
            BigInteger("778828837747666"),
            BigInteger("838487388437847389"),
            BigInteger("1234567892361627372819002399712")
        };

        for (size_t i = 0; i < bigNums.size(); i++) {
            for (size_t j = 0; j < bigNums.size(); j++) {
                if (i == j) CHECK(bigNums[i] == bigNums[j]);
                if (i != j) CHECK(bigNums[i] != bigNums[j]);
                if (i < j)  CHECK(bigNums[i] < bigNums[j]);
                if (i > j)  CHECK(bigNums[i] > bigNums[j]);
                if (i <= j) CHECK(bigNums[i] <= bigNums[j]);
                if (i >= j) CHECK(bigNums[i] >= bigNums[j]);
            }
        }
    }

    BigInteger num = 1234567890;

    SUBCASE("Between BigIntegers(lhs) and Integers(rhs)")
    {
        CHECK((num < 1234567890) == false);
        CHECK((num > 1234567890) == false);
        CHECK((num <= 1234567890) == true);
        CHECK((num >= 1234567890) == true);
        CHECK((num == 1234567890) == true);
        CHECK((num != 1234567890) == false);
    }

    SUBCASE("Between Integers(lhs) and BigIntegers(rhs)")
    {
        CHECK((1234567890 < num) == false);
        CHECK((1234567890 > num) == false);
        CHECK((1234567890 <= num) == true);
        CHECK((1234567890 >= num) == true);
        CHECK((1234567890 == num) == true);
        CHECK((1234567890 != num) == false);
    }

    SUBCASE("Between BigIntegers(lhs) and Strings(rhs)")
    {
        CHECK((num < "1234567890") == false);
        CHECK((num > "1234567890") == false);
        CHECK((num <= "1234567890") == true);
        CHECK((num >= "1234567890") == true);
        CHECK((num == "1234567890") == true);
        CHECK((num != "1234567890") == false);
    }

    SUBCASE("Between Strings(lhs) and BigIntegers(rhs)")
    {
        CHECK(("1234567890" < num) == false);
        CHECK(("1234567890" > num) == false);
        CHECK(("1234567890" <= num) == true);
        CHECK(("1234567890" >= num) == true);
        CHECK(("1234567890" == num) == true);
        CHECK(("1234567890" != num) == false);
    }
}

TEST_CASE("Base Cases For Mutiplication And Division")
{
    BigInteger num = "73217676874632874329498237894378278423786781"
        "87931782837276432872374238489237489328943287482374239834823798423";

    CHECK(num * 0 == 0);
    CHECK(num * 1 == num);
    CHECK(num / num == 1);
    CHECK(num % num == 0);
    CHECK(0 / num == 0);
    CHECK(0 % num == 0);
    CHECK_THROWS(num / 0);
}

TEST_CASE("Random Arithmetic Operations")
{
    std::random_device gen;
    std::uniform_int_distribution<uint64_t> distr((0), (UINT32_MAX));

    for (int i = 0; i < 1; i++)
    {
        uint64_t num1 = distr(gen);
        uint64_t num2 = distr(gen);

        if (num1 == 0 || num2 == 0)
        {
            i--;
            continue;
        }

        if (num1 < num2)
            std::swap(num1, num2);

        BigInteger big1 = num1;
        BigInteger big2 = num2;

        // Addition
        uint64_t result = num1 + num2;
        uint64_t result1 = num1 + num1;
        uint64_t result2 = num2 + num2;

        CHECK(big1 + big2 == result);
        CHECK(big2 + big1 == result);
        CHECK(big1 + num1 == result1);
        CHECK(big1 + num2 == result);
        CHECK(big2 + num1 == result);
        CHECK(big2 + num2 == result2);

        // Subtraction
        result = num1 - num2;
        result1 = num1 - num1;
        result2 = num2 - num2;
        CHECK(big1 - big2 == result);
        CHECK(big1 - num1 == result1);
        CHECK(big1 - num2 == result);
        CHECK(big2 - num2 == result2);

        // Multiplication
        result = num1 * num2;
        result1 = num1 * num1;
        result2 = num2 * num2;
        CHECK(big1 * big2 == result);
        CHECK(big2 * big1 == result);
        CHECK(big1 * num1 == result1);
        CHECK(big1 * num2 == result);
        CHECK(big2 * num1 == result);
        CHECK(big2 * num2 == result2);

        // Division
        result = num1 / num2;
        result1 = num1 / num1;
        result2 = num2 / num2;
        CHECK(big1 / big2 == result);
        CHECK(big2 / big1 == 0);
        CHECK(big1 / num1 == result1);
        CHECK(big1 / num2 == result);
        CHECK(big2 / num1 == 0);
        CHECK(big2 / num2 == result2);

        // Modulo
        result = num1 % num2;
        result1 = num1 % num1;
        result2 = num2 % num2;
        CHECK(big1 % big2 == result);
        CHECK(big2 % big1 == big2);
        CHECK(big1 % num1 == result1);
        CHECK(big1 % num2 == result);
        CHECK(big2 % num1 == num2);
        CHECK(big2 % num2 == result2);
    }
}

TEST_CASE("Operations with zero")
{
    BigInteger num = "12345678901243521772837662516278392";

    CHECK(num + 0 == num);
    CHECK(num - 0 == num);
    CHECK(num * 0 == 0);
    CHECK(num * 0 == 0);
    CHECK(0 / num == 0);
    CHECK(0 % num == 0);

    CHECK_THROWS(0 - num);
    CHECK_THROWS(num / 0);
}

TEST_CASE("Operator chaining")
{
    SUBCASE("Chaining addition and subtraction")
    {
        BigInteger num1 = 13141516;
        BigInteger num2 = 101112;
        BigInteger num3 = 56789;
        BigInteger num4 = 1234;

        CHECK(num1 + num2 + num3 + num4 == 13300651);
        CHECK(num1 - num2 + num3 + num4 == 13098427);
        CHECK(num1 + num2 - num3 + num4 == 13187073);
        CHECK(num1 - num2 - num3 + num4 == 12984849);
    }

    SUBCASE("Chaining mutiplication, divison and modulo")
    {
        BigInteger num1 = 313233343536373839U;
        BigInteger num2 = 212223242526272829U;
        BigInteger num3 = 111213141516171819U;
        BigInteger num4 = 12345678910U;

        CHECK(num1 * num2 / num3 % num4 == 9021418300U);
        CHECK(num1 * num2 % num3 / num4 == 6392448);
        CHECK(num1 / num2 * num3 % num4 == 6635659579U);
        CHECK(num1 / num2 % num3 * num4 == 12345678910U);
        CHECK(num1 % num2 * num3 / num4 == "909925710857417206643517");
        CHECK(num1 % num2 / num3 * num4 == 0);
    }
}

TEST_CASE("Addition With Big Numbers")
{
    BigInteger n1, n2;

    n1 = "62612523485743887543857346786734436756473657827638987427384723485743887543"
        "8573467867344367564736578276389874273847238478289573479583847828957347958163"
        "152267351256436732657436267491881783128778376234736326453675162635126351632645"
        "326721793128372193721847328746578632948239463254673528741687126478237645267";
    n2 = "771278378127312635123672173786542060529704217132357214772959828385120983424"
        "339263541090375634996368065850294867611447397165152437998796443501783597014569"
        "84067168313210331303669787440432347511637996556242776045622241233979589718916127"
        "6381728356167543675271837127381623791273512677817621635162632131273861237612836217"
        "836128387621863128763721863712367812678314883942181258162736721837829173817289378947821635";
    CHECK((n1 + n2) == "771278378127312635123672173786542060529704217132357214772959"
        "828385120983424339263541090375634996368066476420102468886272"
        "603725905866140811066520175290959714945530366960751912136447"
        "872190819483944702140704150330496259578274186762640860229557"
        "313515501838424104478638060523930948945309503418291410507508"
        "597564066511380471254739254508455485514992084561534525643630"
        "520814206402199976511357915504415857185466902");

    n1 = "3723847893257835478943875782378926372675479249878765676543456788765456789"
        "8765621789039876152673892187635167283716253671892387651236712837615263782176"
        "35412578372156317276354215625167215637215672135462713652173625163782165321783"
        "6521673812567312567524136762571256367126357215678712677352752176352176532715675"
        "37261563712635712536127346521456125342165142634215342563245164356241534521531245"
        "632153521436512532456134521435621465312543126543521465234521635216342156432643125643521635214356214";
    n2 = "8327948273879423784372894328623478472368472367482365432567432676423674576325432674673267237846738267"
        "823647236487236742638746723847632784327646873274367842378463724678237847623847263784678267482364826378"
        "4623876487236498327489237988234819272197312712879128371297323987324983248973290327048238047237894038202"
        "305032502000000000000230923482034893204284320402349302849238230984203824";
    CHECK(n1 + n2 == "372384789325783547894387578237892637267547924987876567654345678876"
        "545678987656217890398761526738921876351681165110810598347660885261456999631110190"
        "23590874020264819916031184523709304699639624048078411810348518616361470376892100396"
        "3461973305910002888240900080406049727296835024974397374785548200032809521580104618758"
        "912294903445483575944436016143733994692822169161646168022885950478021892248056967455977"
        "0350172723740653967312543126543752388716556528420626476834992428492759866198560038");
}

TEST_CASE("Subtraction with Big Numbers")
{
    BigInteger num1, num2;

    num1 = "37238478932578300000000000000054789438757823789263726754792498787656765434567887654567898765621789039"
        "876152673892187635167283716253671892387651236712837615263782176354125783721563172763542156251672156372156"
        "72135462713652173625163782165321783652167381256731256752413676257125636712635721567871267735275217635217653"
        "2715675372615637126357125361273465214561253421651426342153425632451643562415345215312456321535214365125324561"
        "34521435621465312543126543521465234521635216342156432643125643521635214356214";
    num2 = "8327876657657546345343645757697899807686577567567263784678267482364826378462387648723649832748923324223342"
        "3424212312312312321798823481903987615267389218763516728371625363243243999000000718876512367263782176354125783"
        "7215631727635421562516721563721567213546271365217362516378216532178365216738125673125675241367625712563671263"
        "5721567871267735275217635217653272721973127128791283712973239873249832489732903270482380472378940382023050325"
        "02000000000000230923482034893204284320402349302849238230984203824";
    CHECK(num1 - num2 == "289106022749207536546563542423568896310712462216964629701142313052919390561055000058442489"
        "3287286571565281033147095640393605153637132370198888970997391573891210933919158945939716327276347026860043542"
        "9993939085942048764365418975283606028050620884458140431849853871963137407474201804573563511331420621495423938"
        "5002755900386599799155584858576259982475793436001489294532150246343487386703225420953828227234182647319846529"
        "4562096319130588963312543126543290541752486742012057836030293822794283404230152390");

    num1 = "839724789238794879237984239846782367847623765423647832753267834789"
        "3485743887543857346786734436756473657827638987427384723847828957347958"
        "7346578434678376534853476583";
    num2 = "762376427367472364238948923894294932949237432747276181982832823742348"
        "2374327483958947854389572893843724238792758743854798547398457347284238947"
        "329874329742348932";
    CHECK(num1 - num2 == "83964855159605813200156034495439293835"
        "43288416803731051350695515069743395650111108950891948997799184273455215108151"
        "5103383679742175022240062339487348502205111127651");

    CHECK_THROWS(num2 - num1);
}

TEST_CASE("Mutiplication With Big Numbers")
{
    BigInteger num1, num2;

    num1 = "2178903987615267389218763516728371625367189238765123671283761526378"
        "21763541257837215631727635004215625167215637215672135462713652173625163"
        "78216532178365216738125673125675241367625712563671263572156787126773527"
        "52176352176532715675372615637126357125361273465214561253421651426342153"
        "42563245164356241534521531245632153521436512532456134521435621465312543"
        "126543521465234521635216342156432643125643521635214356214";
    num2 = "8243422342343278487348975789437895437685678348796543897382798793289"
        "73276892367468235643862779863248419029902194219401412041472304234232674"
        "832673782665324042504202750624057425964425432432940252457402";
    CHECK(num1 * num2 == "1796162581332855736153718556607202293479450180765091"
        "19063205327919029517268458650835663577939375441559280592548714155416168"
        "77987559629163200758085404931153675252526390256365177542308889007179772"
        "38761489799308095955700556059189868117009238384149170169162483043434145"
        "17696543127984369674125390843764271081633941791372141026314175691043779"
        "21767645066997173219860855652185344879055350590076324290198160843078401"
        "81358584508378358623533372988494486629641916251405777837909611949365214"
        "37113840412717481988322035221061180062498104086965738206362430913361292"
        "377982587499148248029863863318604161587984297242888996028");

    num1 = "182783128372173678128673843834543721372022934794501807650911906320532"
        "791902951726845865083566357793937544155928059254871415541616877987559629"
        "163200758085404931153675252526390256365177542308889007179772387614897993"
        "080959557005560591898681170092383841491701691624830434341451769654312798"
        "436967412539084376427108163394179137214102631417569104377921767645066997"
        "173219860855652185344879055350590076324290198160843078401813585845083783"
        "586235333729884944866296419162514057778379096119493652143711384041271748"
        "198832203522106118006249810408696573820636243091336129237798258749914824"
        "8029863863318604161587984297242888996028";
    num2 = "237646783276843276427563874297404230770544054702407542943202430425642"
        "982598765435678908765435678908761789209817902876328372873278732687827138"
        "921321309128371290319031287312839120398723643575824912837623482374821930"
        "394874823767846275849804982018376273286587465987302901283732463258734623"
        "79480289310279127376348256324632784678326478019871289871621982178921712";
    CHECK(num1 * num2 == "4343782249492538145878936986064168343246186063557415349"
        "924965548996960481487883913749978445324525745322278210679898789431226655"
        "929251537976854255509441780453150464775171594330092262631109752707263947"
        "490167659152055672402753848480094820630917515833209127156565257897156036"
        "652429083900242408874510202336137483607214952032734178178991043910207384"
        "045303450664075548044262360956357303263415971542907528659296843106030467"
        "352645217836235543280303425999670381874298190441863448349969521139740852"
        "543398350725225435989973350487770686818944603402990818572720033779185607"
        "305392979188578746306729093639101047454932214331168572824211795379183577"
        "112649798548597077883337043804791380037779886878425958017085385362134808"
        "851867677560992513475278841537541065520481681828803029097861009348529962"
        "274485289349322814619825402519949099858936941318508476824308511997808464"
        "482457384103422031418631747055741831866481366313516178302963886616090700"
        "2806368846419270378408363071307730066491290959936");
}

TEST_CASE("Division With Big Numbers")
{
    BigInteger num1, num2;

    num1 = "182783128372173678128673843834543721372022934794501807650911906320532"
        "791902951726845865083566357793937544155928059254871415541616877987559629"
        "163200758085404931153675252526390256365177542308889007179772387614897993"
        "080959557005560591898681170092383841491701691624830434341451769654312798"
        "436967412539084376427108163394179137214102631417569104377921767645066997"
        "173219860855652185344879055350590076324290198160843078401813585845083783"
        "586235333729884944866296419162514057778379096119493652143711384041271748"
        "198832203522106118006249810408696573820636243091336129237798258749914824"
        "8029863863318604161587984297242888996028";
    num2 = "237646783276843276427563874297404230770544054702407542943202430425642"
        "982598765435678908765435678908761789209817902876328372873278732687827138"
        "921321309128371290319031287312839120398723643575824912837623482374821930"
        "394874823767846275849804982018376273286587465987302901283732463258734623"
        "79480289310279127376348256324632784678326478019871289871621982178921712";
    CHECK(num1 / num2 == "7691378181174160667544179711293685858402249586009789364"
        "873435111556938385689768148601690831952508659281768434815031004909426307"
        "206425160662075291976168165097915067717537426081414347346578778812860833"
        "8515362244574612330512844074672382555108281001539742582779");

    num1 = "376247496237657684315768543752677673682765432786954678679378423875436"
        "7589672387437834574863532765732562787432647328657326589278348973248623782"
        "36578236589273895762837564957823647823635772862378472375672836587632753290"
        "237857644870813463729432084732657237498832472363279758283075723659238523987"
        "5237865723979523857692738572398572365782365739278378759283753289572736573265"
        "792365792738957283957892357329857289537289537853295729958732895732895372953725892";
    num2 = "8836432424234234923493294328429348323285928398539285325829583295238";

    CHECK(num1 / num2 == "42579117699784060266689778966463703394683943736740629137147415"
        "57382313136600620134247633402721247550095791145049781066195545616270625872851140"
        "334985123542183239773003573290619836828051171124138384729910060272887300692494616"
        "7862229728193403339170714759851716579938038169948546273739751701892534867881404924"
        "1261072233547403023244830866068203152732770744588011157550049954991132170884");
}

TEST_CASE("Modulo With Big Numbers")
{
    BigInteger num1, num2;

    num1 = "182783128372173678128673843834543721372022934794501807650911906320532"
        "791902951726845865083566357793937544155928059254871415541616877987559629"
        "163200758085404931153675252526390256365177542308889007179772387614897993"
        "080959557005560591898681170092383841491701691624830434341451769654312798"
        "436967412539084376427108163394179137214102631417569104377921767645066997"
        "173219860855652185344879055350590076324290198160843078401813585845083783"
        "586235333729884944866296419162514057778379096119493652143711384041271748"
        "198832203522106118006249810408696573820636243091336129237798258749914824"
        "8029863863318604161587984297242888996028";
    num2 = "237646783276843276427563874297404230770544054702407542943202430425642"
        "982598765435678908765435678908761789209817902876328372873278732687827138"
        "921321309128371290319031287312839120398723643575824912837623482374821930"
        "394874823767846275849804982018376273286587465987302901283732463258734623"
        "79480289310279127376348256324632784678326478019871289871621982178921712";
    CHECK(num1 % num2 == "1317043755639313464036287361753096574896205697714283822"
        "178645154126360984626258023898347772283209637965749106651743005881524541"
        "957967820451792658457508491845287527107206785603141810985899632925980666"
        "302429162429929311153106296206688005220138135507728767709560355614193541"
        "588431700180978810474071081694532514956980379684081303405467276551278787"
        "0314768598380");

    num1 = "789423784327864326784326784786327636784537685346754877834958793489745"
        "786954379865437664783764578625347824879478594378965678476857642675672479"
        "654762576467436785437682576428975764236785476823765843672876572456783289"
        "745798427635768324765893748757684327853298589624954295423805342050934980"
        "598043298054908895438904239089898198139804980398439018409831098498039804"
        "893129840983812894890128899804109898049804890190980509849853205893427852"
        "784327878542787485287578879278597852879587489758279892798875299852895781"
        "038109489312804081389478548285042785402758432754329758244285420857989324"
        "587892378579287589432753984789532729352898237593427985347584397582349783"
        "427853247853405034954329058420430285425";
    num2 = "983748793287942839748932879498348534792597843289589429858324785392457"
        "342895739842578934275348957428358943895437289437892781890419093104990814"
        "890285875982758429745897289575894372895734298753849275893478943279857438"
        "927589473857324985783429578234679652786742578634278567483782729845789572"
        "849472989578495842789478932478942749275987428957398547298572894532789743"
        "285764275642875784927589718971897141764786475268747592784359827895274285"
        "2476184729827";
    CHECK(num1 % num2 == "5098313775582611359415549038671382167850870990016727168"
        "905530993215834729793283406969034170685479697508568791420757854538699967"
        "049338842752474310697439344682978934760982670889064979321341150795025254"
        "207772207209161527487507336183645662898844880739678882284887513177697049"
        "154587400518050618494530959993956090071580768530174871020029416620757456"
        "029569894804373161718113838781993037074239671457662861435146835793305786"
        "906550334903132250759782882");
}