rfc1918_reverse_zones = [
        "c.f.ip6.arpa",
        "d.f.ip6.arpa",
        "168.192.in-addr.arpa",
        ] + ["{}.172.in-addr.arpa".format(octet) for octet in range(16, 32)] + [
        "10.in-addr.arpa"]

print("struct string_buffer rfc1918_reverse_zones[] = {")
for i in rfc1918_reverse_zones:
    print("   {.string = \"" + i + "\", .length = " + str(len(i)) + "},")


print("};")
print("size_t reverse_zones_len = " + str(len(rfc1918_reverse_zones)))
