import os

import discord
from discord.ext import commands
from dotenv import load_dotenv

load_dotenv()

intents = discord.Intents.all()
bot = commands.Bot(intents=intents, command_prefix="!")
tree = bot.tree


# https://qiita.com/t3tra/items/05b7b7389d6f1b9de11a
@bot.event
async def on_ready():
    print(f"Logged in as {bot.user.name}.")  # type: ignore
    await tree.sync()
    print("Synced slash commands.")


@tree.command(name="test", description="test command")
async def test(ctx: discord.Interaction):
    text = "test"
    embed = discord.Embed(title="test", description=text)
    await ctx.response.send_message(embed=embed)


bot.run(os.getenv("DISCORD_TOKEN"))  # type: ignore
