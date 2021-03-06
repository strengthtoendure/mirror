topic "HttpCookie";
[2 $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9;2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,2#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:item]
[l288;a4;*@5;1 $$6,6#70004532496200323422659154056402:requirement]
[l288;i1121;b17;O9;~~~.1408;2 $$7,0#10431211400427159095818037425705:param]
[i448;b42;O9;2 $$8,8#61672508125594000341940100500538:tparam]
[b42;2 $$9,9#13035079074754324216151401829390:normal]
[{_} 
[ {{10000@(113.42.0) [s0;%% [*@7;4 HttpCookie]]}}&]
[s4; &]
[s1;:HttpCookie`:`:struct: [@(0.0.255)3 struct][3 _][*3 HttpCookie][3 _:_][@(0.0.255)3 public][3 _
][*@3;3 Moveable][3 <][*3 HttpCookie][3 >_]&]
[s2;%% This simple structure is used to store information about HTTP 
Cookie.&]
[s0;i448;a25;kKO9;:noref:@(0.0.255) &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Member List]]}}&]
[s3; &]
[s5;:HttpCookie`:`:id: [_^String^ String]_[* id]&]
[s2;%% The name of cookie.&]
[s3; &]
[s4; &]
[s5;:HttpCookie`:`:value: [_^String^ String]_[* value]&]
[s2;%% Value of cookie.&]
[s3; &]
[s4; &]
[s5;:HttpCookie`:`:domain: [_^String^ String]_[* domain]&]
[s2;%% Cookie domain.&]
[s3; &]
[s4; &]
[s5;:HttpCookie`:`:path: [_^String^ String]_[* path]&]
[s2;%% Cookie path.&]
[s3; &]
[s4; &]
[s5;:HttpCookie`:`:raw: [_^String^ String]_[* raw]&]
[s2;%% Unparsed cookie.&]
[s3; &]
[s4; &]
[s5;:HttpCookie`:`:Clear`(`): [@(0.0.255) void]_[* Clear]()&]
[s2;%% Clears all member variables.&]
[s3; &]
[s4; &]
[s5;:HttpCookie`:`:Parse`(const String`&`): [@(0.0.255) bool]_[* Parse]([@(0.0.255) const]_
[_^String^ String][@(0.0.255) `&]_[*@3 cookie])&]
[s2;%% Parses [%-*@3 cookie] into member variables `- expects the format 
of value of HTTP `"Set`-Cookie`" header.&]
[s3;%% ]